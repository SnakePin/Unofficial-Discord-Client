#include "discord/discord.hpp"
#include "discord/token.hpp"
#include "discord/packets.hpp"
#include "discord/gateway_opcodes.hpp"
#include "discord/utils.hpp"

#include "sws/client_wss.hpp"

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

#include <memory>
#include <algorithm>
#include <functional>
#include <string_view>

using namespace Discord;
using namespace Utils;

Client::Client(std::string& token, AuthTokenType tokenType)
	: token(token, tokenType), //Make sure token is set before httpAPI is initialized
	sessionID(""),
	heartbeatInterval(40000),
	websocket("gateway.discord.gg/?v=6&encoding=json", false),
	sequenceNumber(0),
	userAgent(DefaultUserAgentString),

	// When we pass *this to HTTP_API_CLASS's constructor it will call the Client::HTTP_API_CLASS::HTTP_API_CLASS(const Client& clientObj)
	// This means HTTP_API_CLASS will have reference to the outer class to allow it to access things like token
	httpAPI(*this)
{
	io_context = std::make_shared<asio::io_context>();
	heartbeatTimer = std::make_unique<asio::steady_timer>(*io_context);
}

std::string Client::GenerateIdentifyPacket(bool compress) {
	rapidjson::Document document;

	rapidjson::Pointer("/op").Set(document, GatewayOpcodes::Identify);
	rapidjson::Pointer("/d/token").Set(document, this->token.value.c_str());
	rapidjson::Pointer("/d/compress").Set(document, compress);

	rapidjson::Pointer("/d/properties/os").Set(document, "Linux");
	rapidjson::Pointer("/d/properties/browser").Set(document, "Chrome");
	rapidjson::Pointer("/d/properties/device").Set(document, "");
	rapidjson::Pointer("/d/properties/browser_user_agent").Set(document, userAgent.c_str());
	rapidjson::Pointer("/d/properties/browser_version").Set(document, "");
	rapidjson::Pointer("/d/properties/os_version").Set(document, "");
	rapidjson::Pointer("/d/properties/referrer").Set(document, "");
	rapidjson::Pointer("/d/properties/referring_domain").Set(document, "");
	rapidjson::Pointer("/d/properties/referrer_current").Set(document, "");
	rapidjson::Pointer("/d/properties/referring_domain_current").Set(document, "");

	rapidjson::Pointer("/d/properties/release_channel").Set(document, "stable");
	rapidjson::Pointer("/d/properties/client_build_number").Set(document, 40393);
	rapidjson::Pointer("/d/properties/client_event_source").Set(document, rapidjson::Value(rapidjson::kNullType));

	rapidjson::Pointer("/d/presence/status").Set(document, "online");
	rapidjson::Pointer("/d/presence/since").Set(document, 0);
	rapidjson::Pointer("/d/presence/activities").Set(document, rapidjson::Value(rapidjson::kArrayType)); // TODO implement activities
	rapidjson::Pointer("/d/presence/afk").Set(document, false);
	
	return JsonDocumentToJsonString(document);
}

std::string Client::GenerateResumePacket(std::string& sessionID, uint32_t sequenceNumber) {
	rapidjson::Document document;

	rapidjson::Pointer("/op").Set(document, GatewayOpcodes::Resume);
	rapidjson::Pointer("/d/session_id").Set(document, sessionID.c_str());
	rapidjson::Pointer("/d/token").Set(document, this->token.value.c_str());
	rapidjson::Pointer("/d/seq").Set(document, sequenceNumber);

	return JsonDocumentToJsonString(document);
}

std::string Client::GenerateGuildChannelViewPacket(const Snowflake& guild, const Snowflake& channel) {
	// Produce a packet that looks like scripts/outbound_packets/op14.json

	std::string gid = std::to_string(guild.value);
	std::string cid = std::to_string(channel.value);

	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Pointer("/op").Set(document, 14); // GUILD_SWITCH
	rapidjson::Pointer("/d/guild_id").Set(document, gid.c_str());
	rapidjson::Pointer("/d/typing").Set(document, true);
	rapidjson::Pointer("/d/activities").Set(document, true);

	rapidjson::Value inn(rapidjson::kArrayType);
	inn.PushBack(0, allocator);
	inn.PushBack(99, allocator);
	rapidjson::Value arr(rapidjson::kArrayType);
	arr.PushBack(inn, allocator);

	rapidjson::Value obj(rapidjson::kObjectType);
	obj.AddMember(rapidjson::Value(cid.c_str(), allocator), arr, allocator);

	rapidjson::Pointer("/d/channels").Set(document, obj);

	return JsonDocumentToJsonString(document);
}

void Client::SendHeartbeatAndResetTimer(const asio::error_code& error) {
	if (!error) {
		connection->send("{\"op\":1,\"d\":" + std::to_string(sequenceNumber) + "}");
		std::cout << "Client: Sending heartbeat.\n";

		heartbeatTimer->expires_from_now(std::chrono::milliseconds(heartbeatInterval - 2000));
		heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this, std::placeholders::_1));
	}
}

void Client::SendIdentify() {
	std::cout << "Client: Sending identify packet.\n";
	connection->send(GenerateIdentifyPacket());
}

void Client::SendResume(std::string& sessionID, uint32_t sequenceNumber) {
	this->sessionID = sessionID;
	connection->send(GenerateResumePacket(sessionID, sequenceNumber));
}

void Client::ProcessHello(rapidjson::Document& document) {
	rapidjson::Value eventData = document["d"].GetObject();
	heartbeatInterval = eventData["heartbeat_interval"].GetInt();

	heartbeatTimer->expires_from_now(std::chrono::milliseconds(heartbeatInterval - 2000));
	heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this, std::placeholders::_1));

	std::cout << "Client: Received HELLO packet. Heartbeat interval: " << heartbeatInterval << std::endl;

	OnHelloPacket();
}

void Client::OnHelloPacket() {
	SendIdentify();
}

void Client::ProcessReady(rapidjson::Document& document) {
	ReadyPacket packet = ReadyPacket::LoadFrom(document, "/d");
	sessionID = packet.sessionID;
	OnReadyPacket(packet);
}

void Client::ProcessGuildCreate(rapidjson::Document& document) {
	OnGuildCreate(Guild::LoadFrom(document, "/d"));
}

void Client::ProcessMessageCreate(rapidjson::Document& document) {
	OnMessageCreate(Message::LoadFrom(document, "/d"));
}

void Client::Run() {
	// Start constructing the websocket events:
	// on_message, on_open, on_close, on_error
	websocket.on_message = [this](std::shared_ptr<WssClient::Connection> /* connection */, std::shared_ptr<WssClient::InMessage> in_message) {
		// string() can only be called once because it consumes the stream buffer, so make sure to store the return value!
		std::string response = in_message->string();

		rapidjson::Document document;
		document.Parse(response.c_str());
		GatewayOpcodes opcode = (GatewayOpcodes)document["op"].GetInt();

		if (opcode == GatewayOpcodes::Hello) { // HELLO
			ProcessHello(document);

		}
		else if (opcode == GatewayOpcodes::InvalidSession) { // Error: resume failed.
			std::cout << "Resume failed.\n";
			std::cout << response << std::endl;
		}
		else if (opcode == GatewayOpcodes::HeartbeatAck) {
			std::cout << "Client: Heartbeat acknowledged by server.\n";
		}
		else if (opcode == GatewayOpcodes::Dispatch) { // DISPATCH
			std::string eventName = document["t"].GetString();

			if (document["s"].IsUint64())
				sequenceNumber = std::max(sequenceNumber, document["s"].GetUint64());

			if (eventName == "GUILD_CREATE") {
				ProcessGuildCreate(document);

			}
			else if (eventName == "READY") {
				ProcessReady(document);

			}
			else if (eventName == "MESSAGE_CREATE") {
				ProcessMessageCreate(document);

			}
			else if (eventName == "MESSAGE_REACTION_ADD") {
				OnMessageReactionAdd(MessageReactionPacket::LoadFrom(document, "/d"));

			}
			else if (eventName == "MESSAGE_REACTION_REMOVE") {
				OnMessageReactionRemove(MessageReactionPacket::LoadFrom(document, "/d"));

			}
			else if (eventName == "TYPING_START") {
				OnTypingStart(TypingStartPacket::LoadFrom(document, "/d"));

			}
			else if (eventName == "GUILD_MEMBER_LIST_UPDATE") {
				OnGuildMemberListUpdate(GuildMemberListUpdatePacket::LoadFrom(document, "/d"));

			}
			else if (eventName == "RESUMED") {
				std::cout << "Session resumed!" << std::endl;

			}
			else {
				std::cout << "Client: Message received: \"" << response << "\"" << std::endl;

			}

		}
		else {
			std::cout << "Client: Message received: \"" << response << "\"" << std::endl;

		}
	};

	websocket.on_open = [this](std::shared_ptr<WssClient::Connection> connection) {
		std::cout << "Client: Opened connection to " << connection->remote_endpoint_address() << std::endl;
		this->connection = connection;
	};

	websocket.on_close = [this](std::shared_ptr<WssClient::Connection> /*connection*/, int status, const std::string& /*reason*/) {
		std::cout << "Client: Closed connection with status code " << status << std::endl;
		heartbeatTimer->cancel();
	};

	websocket.on_error = [](std::shared_ptr<WssClient::Connection> /*connection*/, const SimpleWeb::error_code& ec) {
		std::cout << "Client: Error: " << ec << ", error message: " << ec.message() << std::endl;
	};

	//Reset io_service if it was ran before
	if (io_context->stopped()) {
		//io_context must be resetted in prior to run()
		io_context->reset();
	}
	
	//Set io_service to the shared io_context
	websocket.io_service = this->io_context;
	
	websocket.start();

	//Start io_service
	io_context->run();

}

void Client::OpenGuildChannelView(const Snowflake& guild, const Snowflake& channel) {
	ScheduleNewWSSPacket(GenerateGuildChannelViewPacket(guild, channel));
}

void Client::OpenPrivateChannelView(const Snowflake& channel) {
	// TODO this isn't working - reason is yet to be determined
	return;

	// Yes, this packet really is that short.
	/*  {
			"op": 13,
			"d": {
				"channel_id": "593804939407392793"
			}
		}
	*/
	rapidjson::Document document;
	rapidjson::Pointer("/op").Set(document, 13);
	rapidjson::Pointer("/d/channel_id").Set(document, std::to_string(channel.value).c_str());

	ScheduleNewWSSPacket(JsonDocumentToJsonString(document));
}

void Client::UpdatePresence(std::string& status) {
	rapidjson::Document document;
	rapidjson::Pointer("/op").Set(document, GatewayOpcodes::StatusUpdate);
	rapidjson::Pointer("/d/status").Set(document, status.c_str());
	rapidjson::Pointer("/d/since").Set(document, 0);
	rapidjson::Pointer("/d/afk").Set(document, false);
	rapidjson::Pointer("/d/activities").Set(document, rapidjson::Value(rapidjson::kArrayType));

	ScheduleNewWSSPacket(JsonDocumentToJsonString(document));
}

void Client::ScheduleNewWSSPacket(std::string_view out_message_str, const std::function<void(const std::error_code&)>& callback, unsigned char fin_rsv_opcode) {
	asio::post(*websocket.io_service, [=] {
		connection->send(out_message_str, callback, fin_rsv_opcode);
	});
}