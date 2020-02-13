#include "discord/discord.hpp"
#include "discord/token.hpp"
#include "discord/packets.hpp"
#include "discord/gateway_opcodes.hpp"
#include "discord/InternalUtils.hpp"

#include <cpprest/ws_client.h>

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

#include <atomic>
#include <memory>
#include <map>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <functional>
#include <string>

#undef GetObject

using namespace Discord;
using namespace InternalUtils;

using utility::conversions::to_string_t;
using utility::conversions::to_utf8string;
using web::web_sockets::client::websocket_incoming_message;
using web::web_sockets::client::websocket_outgoing_message;
using web::web_sockets::client::websocket_close_status;

static pplx::task<void> CasabalancaWebSocketSendUtf8StringAsync(Discord::WssClient& wsclient, const std::string& str);

Client::Client(const std::string& authToken_arg, AuthTokenType tokenType)
	: authToken(authToken_arg, tokenType), //Make sure token is set before httpAPI is initialized
	heartbeatInterval(40000),
	userAgent(DefaultUserAgentString),

	// When we pass *this to HTTP_API_CLASS's constructor it will call the Client::HTTP_API_CLASS::HTTP_API_CLASS(const Client& clientObj)
	// This means HTTP_API_CLASS will have reference to the outer class to allow it to access things like token
	httpAPI(*this)
{

}

std::string Client::GenerateIdentifyPacket(bool compress) {
	rapidjson::Document document;

	rapidjson::Pointer("/op").Set(document, GatewayOpcodes::Identify);
	rapidjson::Pointer("/d/token").Set(document, authToken.value);
	rapidjson::Pointer("/d/compress").Set(document, compress);

	rapidjson::Pointer("/d/properties/os").Set(document, "Linux");
	rapidjson::Pointer("/d/properties/browser").Set(document, "Chrome");
	rapidjson::Pointer("/d/properties/device").Set(document, "");
	rapidjson::Pointer("/d/properties/browser_user_agent").Set(document, to_utf8string(userAgent));
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

std::string Client::GenerateResumePacket(std::string& sessionID_arg, uint32_t sequenceNumber_arg) {
	rapidjson::Document document;

	rapidjson::Pointer("/op").Set(document, GatewayOpcodes::Resume);
	rapidjson::Pointer("/d/session_id").Set(document, sessionID_arg);
	rapidjson::Pointer("/d/token").Set(document, authToken.value);
	rapidjson::Pointer("/d/seq").Set(document, sequenceNumber_arg);

	return JsonDocumentToJsonString(document);
}

std::string Client::GenerateGuildChannelViewPacket(const Snowflake& guild, const Snowflake& channel) {
	// Produce a packet that looks like scripts/outbound_packets/op14.json

	std::string gid = std::to_string(guild.value);
	std::string cid = std::to_string(channel.value);

	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Pointer("/op").Set(document, 14); // GUILD_SWITCH
	rapidjson::Pointer("/d/guild_id").Set(document, gid);
	rapidjson::Pointer("/d/typing").Set(document, true);
	rapidjson::Pointer("/d/activities").Set(document, true);

	rapidjson::Value inn(rapidjson::kArrayType);
	inn.PushBack(0, allocator);
	inn.PushBack(99, allocator);
	rapidjson::Value arr(rapidjson::kArrayType);
	arr.PushBack(inn, allocator);

	rapidjson::Value obj(rapidjson::kObjectType);
	obj.AddMember(rapidjson::Value(cid, allocator), arr, allocator);

	rapidjson::Pointer("/d/channels").Set(document, obj);

	return JsonDocumentToJsonString(document);
}

void Client::SendHeartbeatAndResetTimer() {
	CasabalancaWebSocketSendUtf8StringAsync(websocket, "{\"op\":1,\"d\":" + std::to_string(sequenceNumber) + "}").wait();
	std::cout << "Client: Sending heartbeat." << std::endl;

	heartbeatTimer.doLater(heartbeatInterval - 2000, [this]() {
		SendHeartbeatAndResetTimer();
	});
}

void Client::SendIdentify() {
	CasabalancaWebSocketSendUtf8StringAsync(websocket, GenerateIdentifyPacket()).wait();
}

void Client::SendResume(std::string& sessionID_arg, uint32_t sequenceNumber_arg) {
	sessionID = sessionID_arg;
	CasabalancaWebSocketSendUtf8StringAsync(websocket, GenerateResumePacket(sessionID_arg, sequenceNumber_arg)).wait();
}

void Client::ProcessHello(rapidjson::Document& document) {
	rapidjson::Value eventData = document["d"].GetObject();
	heartbeatInterval = eventData["heartbeat_interval"].GetInt();

	heartbeatTimer.doLater(heartbeatInterval - 2000, [this]() {
		SendHeartbeatAndResetTimer();
	});

	std::cout << "Client: Received HELLO packet. Heartbeat interval: " << heartbeatInterval << std::endl;

	CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnHelloPacket, this));
}

void Client::ProcessDispatch(rapidjson::Document& document) {
	std::string eventName = document["t"].GetString();

	if (document["s"].IsUint64())
		sequenceNumber = std::max(sequenceNumber, document["s"].GetUint64());

	const std::map<std::string, std::function<void(void)>> dispatchEventHandlers
	{
		{ "GUILD_CREATE", [this, &document]() {
			Guild tmpVar;
			tmpVar.LoadFrom(document["d"]);
			CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnGuildCreate, this, tmpVar));
		}},
		{ "READY", [this, &document]() {
			ProcessReady(document);
		}},
		{ "MESSAGE_CREATE", [this, &document]() {
			Message tmpVar;
			tmpVar.LoadFrom(document["d"]);
			CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnMessageCreate, this, tmpVar));
		}},
		{ "MESSAGE_REACTION_ADD", [this, &document]() {
			MessageReactionPacket tmpVar;
			tmpVar.LoadFrom(document["d"]);
			CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnMessageReactionAdd, this, tmpVar));
		}},
		{ "MESSAGE_REACTION_REMOVE", [this, &document]() {
			MessageReactionPacket tmpVar;
			tmpVar.LoadFrom(document["d"]);
			CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnMessageReactionRemove, this, tmpVar));
		}},
		{ "TYPING_START", [this, &document]() {
			TypingStartPacket tmpVar;
			tmpVar.LoadFrom(document["d"]);
			CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnTypingStart, this, tmpVar));
		}},
		{ "GUILD_MEMBER_LIST_UPDATE", [this, &document]() {
			GuildMemberListUpdatePacket tmpVar;
			tmpVar.LoadFrom(document["d"]);
			CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnGuildMemberListUpdate, this, tmpVar));
		}},
		{ "RESUMED", [this]() {
			CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnResumeSuccess, this));
		}},
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
	packet.LoadFrom(document["d"]);
	sessionID = packet.sessionID;
	CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnReadyPacket, this, packet));
}

void Client::ProcessReconnectPacket(rapidjson::Document& /*document*/) {
	websocket.close().wait();
	websocket.connect(DefaultGatewayURL).wait();
}

void Client::Run() {
	//If already running then return
	if (isRunning) {
		return;
	}

	websocket.set_message_handler([this](websocket_incoming_message msg)
	{
		// handle message from server...
		std::string response = to_utf8string(msg.extract_string().get());

		rapidjson::Document document;
		document.Parse(response);
		GatewayOpcodes opcode = (GatewayOpcodes)document["op"].GetInt();

		if (opcode == GatewayOpcodes::Hello) {
			ProcessHello(document);
		}
		else if (opcode == GatewayOpcodes::Dispatch) {
			ProcessDispatch(document);
		}
		else if (opcode == GatewayOpcodes::InvalidSession) {
			CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnResumeFail, this));
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
	});

	websocket.set_close_handler([this](websocket_close_status /*close_status*/, const utility::string_t& reason, const std::error_code& error)
	{
		//Stop must be called asynchronously or else it won't be able to destruct the websocket because destructor will wait for this handler to finish.
		CallFunctionAsynchronouslyAndForget(std::bind(&Client::Stop, this));
		CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnWSSDisconnect, this, error, to_utf8string(reason)));
	});

	websocket.connect(to_string_t(DefaultGatewayURL)).then([this]()
	{
		isRunning = true;
		CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnWSSConnect, this));
	}).wait();
}

void Client::Stop() {
	//Return if not running
	if (!isRunning) {
		return;
	}

	//Don't forget to change the close handler!
	websocket.set_close_handler([](websocket_close_status, const utility::string_t&, const std::error_code&)
	{
		std::cout << "Client: Connection closed gracefully." << std::endl;
	});

	websocket.close().wait();
	
	//Same websocket_callback_client instance can't be re-used after disconnection.
	websocket = WssClient{};
	
	heartbeatTimer.cancel();

	CallFunctionAsynchronouslyAndForget(std::bind(&Client::OnPostStop, this));

	sequenceNumber = 0;
	isRunning = false;
}

Client::Client(const Client& other)
	: Client(other.authToken.value, other.authToken.tokenType)
{
	*this = other;
}

Client::~Client() {
	Stop();
}

Client& Client::operator=(const Client& other) {
	Stop();
	authToken = other.authToken;
	sessionID = other.sessionID;
	heartbeatInterval = other.heartbeatInterval;
	sequenceNumber = other.sequenceNumber;
	httpAPI.userAgent = other.httpAPI.userAgent;

	return *this;
}

bool Client::IsRunning() {
	return isRunning;
}

void Client::OpenGuildChannelView(const Snowflake& guild, const Snowflake& channel) {
	CasabalancaWebSocketSendUtf8StringAsync(websocket, GenerateGuildChannelViewPacket(guild, channel)).wait();
}

void Client::OpenPrivateChannelView(const Snowflake& /*channel*/) {
	/*
	// TODO this isn't working, reason is yet to be determined
	rapidjson::Document document;
	rapidjson::Pointer("/op").Set(document, 13);
	rapidjson::Pointer("/d/channel_id").Set(document, std::to_string(channel.value));

	CasabalancaWebSocketSendUtf8StringAsync(websocket, JsonDocumentToJsonString(document)).wait();*/
}

void Client::UpdatePresence(std::string& status) {
	rapidjson::Document document;
	rapidjson::Pointer("/op").Set(document, GatewayOpcodes::StatusUpdate);
	rapidjson::Pointer("/d/status").Set(document, status);
	rapidjson::Pointer("/d/since").Set(document, 0);
	rapidjson::Pointer("/d/afk").Set(document, false);
	rapidjson::Pointer("/d/activities").Set(document, rapidjson::Value(rapidjson::kArrayType));

	CasabalancaWebSocketSendUtf8StringAsync(websocket, JsonDocumentToJsonString(document)).wait();
}

static pplx::task<void> CasabalancaWebSocketSendUtf8StringAsync(Discord::WssClient& wsclient, const std::string& str) {
	websocket_outgoing_message msg;
	msg.set_utf8_message(str);

	return wsclient.send(msg);
}

//Begin Default OnEvent function implementations
void Client::OnHelloPacket() {
	SendIdentify();
}
void Client::OnWSSDisconnect(std::error_code errorCode, std::string reason) {
	std::cout << "Client: Closed connection with code " << errorCode << " and the reason being \"" << reason << "\"" << std::endl;
}
void Client::OnWSSConnect() {
	std::cout << "Client: Opened connection to " << to_utf8string(websocket.uri().host()) << std::endl;
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
void Client::OnPostStop() {
	std::cout << "Client: OnPostStop default impl." << std::endl;
}
//End Default OnEvent function implementations