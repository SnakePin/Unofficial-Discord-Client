#include "discord/discord.hpp"
#include "discord/token.hpp"
#include "discord/packets.hpp"
#include "discord/gateway_opcodes.hpp"
#include "discord/utils.hpp"

#include "sws/client_wss.hpp"

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

#include <atomic>
#include <memory>
#include <map>
#include <algorithm>
#include <functional>
#include <string_view>

using namespace Discord;
using namespace Utils;

Client::Client(const std::string& token, AuthTokenType tokenType)
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
		std::cout << "Client: Sending heartbeat." << std::endl;

		heartbeatTimer->expires_from_now(std::chrono::milliseconds(heartbeatInterval - 2000));
		heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this, std::placeholders::_1));
	}
}

void Client::SendIdentify() {
	ScheduleNewWSSPacket(GenerateIdentifyPacket());
}

void Client::SendResume(std::string& sessionID, uint32_t sequenceNumber) {
	this->sessionID = sessionID;
	ScheduleNewWSSPacket(GenerateResumePacket(sessionID, sequenceNumber));
}

void Client::ProcessHello(rapidjson::Document& document) {
	rapidjson::Value eventData = document["d"].GetObject();
	heartbeatInterval = eventData["heartbeat_interval"].GetInt();

	heartbeatTimer->expires_from_now(std::chrono::milliseconds(heartbeatInterval - 2000));
	heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this, std::placeholders::_1));

	std::cout << "Client: Received HELLO packet. Heartbeat interval: " << heartbeatInterval << std::endl;

	OnHelloPacket();
}

void Client::ProcessDispatch(rapidjson::Document& document) {
	std::string eventName = document["t"].GetString();

	if (document["s"].IsUint64())
		sequenceNumber = std::max(sequenceNumber, document["s"].GetUint64());

	const std::map<std::string, std::function<void(void)>> dispatchEventHandlers
	{
		{ "GUILD_CREATE", [this, &document]() { Guild tmpVar; tmpVar.LoadFrom(document, "/d"); OnGuildCreate(tmpVar); }},
		{ "READY", [this, &document]() { ProcessReady(document); }},
		{ "MESSAGE_CREATE", [this, &document]() { Message tmpVar; tmpVar.LoadFrom(document, "/d"); OnMessageCreate(tmpVar); }},
		{ "MESSAGE_REACTION_ADD", [this, &document]() { MessageReactionPacket tmpVar; tmpVar.LoadFrom(document, "/d"); OnMessageReactionAdd(tmpVar); }},
		{ "MESSAGE_REACTION_REMOVE", [this, &document]() { MessageReactionPacket tmpVar; tmpVar.LoadFrom(document, "/d"); OnMessageReactionRemove(tmpVar); }},
		{ "TYPING_START", [this, &document]() { TypingStartPacket tmpVar; tmpVar.LoadFrom(document, "/d"); OnTypingStart(tmpVar); }},
		{ "GUILD_MEMBER_LIST_UPDATE", [this, &document]() { GuildMemberListUpdatePacket tmpVar; tmpVar.LoadFrom(document, "/d"); OnGuildMemberListUpdate(tmpVar); }},
		{ "RESUMED", [this]() { OnResumeSuccess(); }},
	};

	if (dispatchEventHandlers.find(eventName) != dispatchEventHandlers.end()) {
		dispatchEventHandlers.at(eventName)();
	}
	else {
		std::cout << "Client: Unknown dispatch event received: \"" << eventName << "\"" << std::endl;
	}
}

void Client::ProcessReady(rapidjson::Document& document) {
	ReadyPacket packet;
	packet.LoadFrom(document, "/d");
	sessionID = packet.sessionID;
	OnReadyPacket(packet);
}

void Client::ProcessReconnectPacket(rapidjson::Document& document) {
	websocket.stop();
	websocket.start();
}

void Client::Run() {
	//If event loop is already running then return
	if (isRunning) {
		return;
	}

	// Start constructing the websocket events:
	// on_message, on_open, on_close, on_error
	websocket.on_message = [this](std::shared_ptr<WssClient::Connection> /* connection */, std::shared_ptr<WssClient::InMessage> in_message) {
		// string() can only be called once because it consumes the stream buffer, so make sure to store the return value!
		std::string response = in_message->string();

		rapidjson::Document document;
		document.Parse(response.c_str());
		GatewayOpcodes opcode = (GatewayOpcodes)document["op"].GetInt();

		if (opcode == GatewayOpcodes::Hello) {
			ProcessHello(document);
		}
		else if (opcode == GatewayOpcodes::Dispatch) {
			ProcessDispatch(document);
		}
		else if (opcode == GatewayOpcodes::InvalidSession) {
			OnResumeFail();
		}
		else if (opcode == GatewayOpcodes::HeartbeatAck) {
			//ProcessHeartbeatAck();
		}
		else if (opcode == GatewayOpcodes::Reconnect) {
			ProcessReconnectPacket(document);
		}
		else {
			std::cout << "Client: Unknown opcode received: \"" << response << "\"" << std::endl;
		}
	};

	websocket.on_open = [this](std::shared_ptr<WssClient::Connection> connection) {
		this->connection = connection;
		OnWSSConnect();
	};

	websocket.on_close = [this](std::shared_ptr<WssClient::Connection> /*connection*/, int status, const std::string& reason) {
		OnWSSDisconnect(status, reason);
		InternalSignalStop();
	};

	websocket.on_error = [this](std::shared_ptr<WssClient::Connection> /*connection*/, const SimpleWeb::error_code& ec) {
		OnWSSError(ec);
		InternalSignalStop();
	};

	//Reset io_service if it is in stopped status
	if (io_context->stopped()) {
		//io_context must be resetted in prior to run() or else it will stay in stopped status and it won't do work
		io_context->reset();
	}

	//Set io_service of the websocket to the shared io_context and start the websocket
	//Note: websocket won't be able to make any connection until event loop is started
	websocket.io_service = this->io_context;
	websocket.start();

	//This line will make sure run() doesn't return when there are no tasks to do
	//This way the run() will only return when stop() is called
	asio::io_context::work work(*io_context);

	//Start the event loop
	isRunning = true;
	io_context->run();
	isRunning = false;
	eventLoopExit.notify_all();
}

void Client::InternalSignalStop() {
	heartbeatTimer->cancel();
	websocket.stop();
	io_context->stop();
}

void Client::Stop() {
	//Return if not running or else the eventLoopExit condition_variable will create deadlock
	if (!isRunning) {
		return;
	}

	InternalSignalStop();

	std::unique_lock<std::mutex> lock(conditionVariableMutex);
	eventLoopExit.wait(lock, [this]() { return !(isRunning.load()); });
}

Client::Client(const Client& other)
	: Client(other.token.value, other.token.tokenType)
{
	*this = other;
}

Client::~Client() {
	Stop();
}

Client& Client::operator=(const Client& other) {
	Stop();
	token = other.token;
	sessionID = other.sessionID;
	heartbeatInterval = other.heartbeatInterval;
	sequenceNumber = other.sequenceNumber;

	return *this;
}

bool Client::IsRunning() {
	return isRunning.load();
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

void Client::ScheduleNewWSSPacket(std::string out_message_str, const std::function<void(const std::error_code&)> callback, unsigned char fin_rsv_opcode) {
	//We must explicity add the string and the callback (must not be a reference) to capture list of the lambda here
	//or else it would have an invalid reference when it is called by event loop run thread
	//Note: we use std::move on string and callback because we no longer need the local copy of it but on fin_rsv_opcode it doesn't really matter as it has no move ctor
	asio::post(*websocket.io_service, [this, out_message_str = std::move(out_message_str), callback = std::move(callback), fin_rsv_opcode]{
		connection->send(out_message_str, callback, fin_rsv_opcode);
		});
}

#pragma region Default OnEvent function implementations
void Client::OnHelloPacket() {
	SendIdentify();
}
void Client::OnWSSDisconnect(int statusCode, std::string reason) {
	std::cout << "Client: Closed connection with status code " << statusCode << " and the reason being \"" << reason << "\"" << std::endl;
}
void Client::OnWSSError(SimpleWeb::error_code errorCode) {
	std::cout << "Client: Connection closed. Error: " << errorCode << ", error message: " << errorCode.message() << std::endl;
}
void Client::OnWSSConnect() {
	std::cout << "Client: Opened connection to " << connection->remote_endpoint_address() << std::endl;
}
void Client::OnResumeFail() {
	std::cout << "Client: OnResumeFail default impl." << std::endl;
}
void Client::OnResumeSuccess() {
	std::cout << "Client: OnResumeSuccess default impl." << std::endl;
}
void Client::OnReadyPacket(Discord::ReadyPacket packet) {
	std::cout << "Client: OnReadyPacket default impl. Session ID: " << packet.sessionID << std::endl;
}
void Client::OnGuildCreate(Guild guild) {
	std::cout << "Client: OnGuildCreate default impl. Guild ID: " << guild.id.value << std::endl;
}
void Client::OnGuildMemberListUpdate(GuildMemberListUpdatePacket packet) {
	std::cout << "Client: OnGuildMemberListUpdate default impl. Guild ID: " << packet.guildID.value << std::endl;
}
void Client::OnMessageCreate(Message m) {
	std::cout << "Client: OnMessageCreate default impl. Message ID: " << m.id.value << std::endl;
}
void Client::OnTypingStart(TypingStartPacket p) {
	std::cout << "Client: OnTypingStart default impl. User ID: " << p.userID.value << std::endl;
}
void Client::OnMessageReactionAdd(MessageReactionPacket p) {
	std::cout << "Client: OnMessageReactionAdd default impl. User ID: " << p.userID.value << std::endl;
}
void Client::OnMessageReactionRemove(MessageReactionPacket p) {
	std::cout << "Client: OnMessageReactionRemove default impl. User ID: " << p.userID.value << std::endl;
}
void Client::OnReconnectPacket() {
	std::cout << "Client: OnReconnectPacket default impl." << std::endl;
}
#pragma endregion