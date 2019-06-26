#include <discord/discord.hpp>
#include <discord/token.hpp>
#include <discord/packets.hpp>

#include "sws/client_wss.hpp"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>

#include <memory>
#include <algorithm>

using namespace Discord;

Client::Client(std::string token, AuthTokenType tokenType)
    : token(token, tokenType),
	heartbeatInterval(40000),
    websocket("gateway.discord.gg/?v=6&encoding=json", false),
    
	heartbeatTimer(nullptr),
	heartbeatSequenceNumber(0),
	sequenceNumber(0),
	// When we pass *this to HTTP_API_CLASS's constructor it will call the Client::HTTP_API_CLASS::HTTP_API_CLASS(const Client& clientObj)
	// This means HTTP_API_CLASS will have reference to the outer class to allow it to access things like token
	httpAPI(*this) {
	
}

std::string Client::GenerateIdentifyPacket() {
	rapidjson::Document document;
	
	rapidjson::Pointer("/op").Set(document, 2);
	rapidjson::Pointer("/d/token").Set(document, this->token.token.c_str());
	
	rapidjson::Pointer("/d/properties/$os").Set(document, "Linux");
	rapidjson::Pointer("/d/properties/$browser").Set(document, "discord.py");
	rapidjson::Pointer("/d/properties/$device").Set(document, "discord.py");
	rapidjson::Pointer("/d/properties/$referrer").Set(document, "");
	rapidjson::Pointer("/d/properties/$referring_domain").Set(document, "");
	
	rapidjson::Pointer("/d/presence/status").Set(document, "available");
	
	rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
	
	return std::string(buffer.GetString());
}

void Client::SendHeartbeatAndResetTimer(const asio::error_code& error) {
	if (!error) {
		std::string packet = "{\"op\":1,\"d\":" + std::to_string(heartbeatSequenceNumber++) + "}";

		connection->send(packet);
		std::cout << "Client: Sending: " << packet << "\n";

		heartbeatTimer->expires_from_now(std::chrono::milliseconds(heartbeatInterval - 2000));
		heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this, std::placeholders::_1));
	}
}

void Client::ProcessHello(rapidjson::Document &document) {
	rapidjson::Value eventData = document["d"].GetObject();
	heartbeatInterval = eventData["heartbeat_interval"].GetInt();

	heartbeatTimer = new asio::steady_timer(*websocket.io_service);
	heartbeatTimer->expires_from_now(std::chrono::milliseconds(heartbeatInterval - 2000));
	heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this, std::placeholders::_1));

	std::cout << "Received HELLO (opcode 10) packet. Heartbeat interval: " << heartbeatInterval << std::endl;
	
	std::string identify_packet = GenerateIdentifyPacket();
	
	connection->send(identify_packet);
}

void Client::ProcessReady(rapidjson::Document &document) {
	OnReadyPacket(ReadyPacket::LoadFrom(document, "/d"));
}

void Client::ProcessGuildCreate(rapidjson::Document &document) {
	OnGuildCreate(Guild::LoadFrom(document, "/d"));
}

void Client::Run() {
	websocket.on_message = [this](std::shared_ptr<WssClient::Connection> connection, std::shared_ptr<WssClient::InMessage> in_message) {
		std::string response = in_message->string(); // string() can only be called once! see sws/client_ws.hpp for why
	
		rapidjson::Document document;
		document.Parse(response.c_str());
		int opcode = document["op"].GetInt();
		
		if(opcode == 10) { // HELLO
			ProcessHello(document);
		}else if(opcode == 0) { // DISPATCH
			std::string eventName = document["t"].GetString();

			sequenceNumber = std::max(sequenceNumber, document["s"].GetUint64());

			if(eventName == "GUILD_CREATE") {
				ProcessGuildCreate(document);

			}else if(eventName == "READY") {
				ProcessReady(document);
				
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
