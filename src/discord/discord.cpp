#include <discord/discord.hpp>
#include <discord/token.hpp>
#include <discord/packets.hpp>

#include "sws/client_wss.hpp"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/filewritestream.h>

#include <memory>
#include <algorithm>

using namespace Discord;

Client::Client(std::string token, AuthTokenType tokenType)
    : token(token, tokenType),
	sessionID(""),
	heartbeatInterval(40000),
    websocket("gateway.discord.gg/?v=6&encoding=json", false),
	sequenceNumber(0),
    
	// When we pass *this to HTTP_API_CLASS's constructor it will call the Client::HTTP_API_CLASS::HTTP_API_CLASS(const Client& clientObj)
	// This means HTTP_API_CLASS will have reference to the outer class to allow it to access things like token
	httpAPI(*this),
	
	heartbeatTimer(nullptr) {
	
	if(tokenType == AuthTokenType::USER) {
		std::cout << "** Client created in User mode **" << std::endl;
	}

}

std::string Client::GenerateIdentifyPacket() {
	rapidjson::Document document;
	
	rapidjson::Pointer("/op").Set(document, 2);
	rapidjson::Pointer("/d/token").Set(document, this->token.token.c_str());
	rapidjson::Pointer("/d/compress").Set(document, false);
	
	rapidjson::Pointer("/d/properties/os").Set(document, "Linux");
	rapidjson::Pointer("/d/properties/browser").Set(document, "Chrome");
	rapidjson::Pointer("/d/properties/device").Set(document, "");
	rapidjson::Pointer("/d/properties/browser_user_agent").Set(document, httpAPI.userAgent.c_str());
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
	
	rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
	
	return std::string(buffer.GetString());
}

std::string Client::GenerateResumePacket(std::string sessionID, uint32_t sequenceNumber) {
	rapidjson::Document document;
	
	rapidjson::Pointer("/op").Set(document, 6); // RESUME
	rapidjson::Pointer("/d/session_id").Set(document, sessionID.c_str());
	rapidjson::Pointer("/d/token").Set(document, this->token.token.c_str());
	rapidjson::Pointer("/d/seq").Set(document, sequenceNumber);
	
	rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
	
	return std::string(buffer.GetString());
}

void Client::SendHeartbeatAndResetTimer(const asio::error_code& error) {
	if (!error) {
		std::string packet = "{\"op\":1,\"d\":" + std::to_string(sequenceNumber) + "}";

		connection->send(packet);
		std::cout << "Client: Sending: " << packet << "\n";

		heartbeatTimer->expires_from_now(std::chrono::milliseconds(heartbeatInterval - 2000));
		heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this, std::placeholders::_1));
	}
}

void Client::SendIdentify() {
	connection->send(GenerateIdentifyPacket());
}

void Client::SendResume(std::string sessionID, uint32_t sequenceNumber) {
	this->sessionID = sessionID;
	connection->send(GenerateResumePacket(sessionID, sequenceNumber));
}

void Client::ProcessHello(rapidjson::Document &document) {
	rapidjson::Value eventData = document["d"].GetObject();
	heartbeatInterval = eventData["heartbeat_interval"].GetInt();

	heartbeatTimer = new asio::steady_timer(*websocket.io_service);
	heartbeatTimer->expires_from_now(std::chrono::milliseconds(heartbeatInterval - 2000));
	heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this, std::placeholders::_1));

	std::cout << "Received HELLO (opcode 10) packet. Heartbeat interval: " << heartbeatInterval << std::endl;
	
	OnHelloPacket();
}

void Client::OnHelloPacket() {
	SendIdentify();
}

void Client::ProcessReady(rapidjson::Document &document) {
	FILE* fp = fopen("ready.json", "wb");

	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);

	fclose(fp);
	
	ReadyPacket packet = ReadyPacket::LoadFrom(document, "/d");
	sessionID = packet.sessionID;
	OnReadyPacket(packet);
}

void Client::ProcessGuildCreate(rapidjson::Document &document) {
	OnGuildCreate(Guild::LoadFrom(document, "/d"));
}

void Client::ProcessMessageCreate(rapidjson::Document &document) {
	OnMessageCreate(Message::LoadFrom(document, "/d"));
}

void Client::Run() {
	// Start constructing the websocket events:
	// on_message, on_open, on_close, on_error
	websocket.on_message = [this](std::shared_ptr<WssClient::Connection> /* connection */, std::shared_ptr<WssClient::InMessage> in_message) {
		std::string response = in_message->string(); // string() can only be called once! see sws/client_ws.hpp for why
	
		rapidjson::Document document;
		document.Parse(response.c_str());
		int opcode = document["op"].GetInt();
		
		if(opcode == 10) { // HELLO
			ProcessHello(document);

		}else if(opcode == 9) { // Error: resume failed.
			std::cout << "Resume failed.\n";
			std::cout << response << std::endl;

		}else if(opcode == 0) { // DISPATCH
			std::string eventName = document["t"].GetString();

			if(document["s"].IsUint64())
				sequenceNumber = std::max(sequenceNumber, document["s"].GetUint64());

			if(eventName == "GUILD_CREATE") {
				ProcessGuildCreate(document);

			}else if(eventName == "READY") {
				ProcessReady(document);
			
			}else if(eventName == "MESSAGE_CREATE") {
				ProcessMessageCreate(document);

			}else if(eventName == "MESSAGE_REACTION_ADD") {
				OnMessageReactionAdd(MessageReactionPacket::LoadFrom(document, "/d"));

			}else if(eventName == "MESSAGE_REACTION_REMOVE") {
				OnMessageReactionRemove(MessageReactionPacket::LoadFrom(document, "/d"));

			}else if(eventName == "TYPING_START") {
				OnTypingStart(TypingStartPacket::LoadFrom(document, "/d"));

			}else if(eventName == "RESUMED"){
				std::cout << "Session resumed!" << std::endl;

			}else {
				std::cout << "Client: Message received: \"" << response << "\"" << std::endl;

			}

		}else {
			std::cout << "Client: Message received: \"" << response << "\"" << std::endl;

		}
	};

	websocket.on_open = [this](std::shared_ptr<WssClient::Connection> connection) {
		std::cout << "Client: Opened connection to " << connection->remote_endpoint_address() << std::endl;
		this->connection = connection;
	};

	websocket.on_close = [this](std::shared_ptr<WssClient::Connection> /*connection*/, int status, const std::string & /*reason*/) {
		std::cout << "Client: Closed connection with status code " << status << std::endl;
		heartbeatTimer->cancel();
	};

	websocket.on_error = [](std::shared_ptr<WssClient::Connection> /*connection*/, const SimpleWeb::error_code &ec) {
		std::cout << "Client: Error: " << ec << ", error message: " << ec.message() << std::endl;
	};
	
	websocket.start();
}

std::string Client::GenerateGuildChannelViewPacket(const Snowflake &guild, const Snowflake &channel) {
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

	rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

	return std::string(buffer.GetString());
}

void Client::OpenGuildChannelView(const Snowflake &guild, const Snowflake &channel) {
	std::string packet = GenerateGuildChannelViewPacket(guild, channel);

	asio::post(*websocket.io_service, [=] {
		connection->send(packet);
	});
}