#include "discord/discord.hpp"

#include "sws/client_wss.hpp"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>

#include <memory>

using namespace Discord;

Client::Client(std::string token, bool bot)
    : token(token), bot(bot),
	heartbeatInterval(40000),
    websocket("gateway.discord.gg/?v=6&encoding=json", false),
    
	heartbeatTimer(nullptr),
	heartbeatSequenceNumber(0) {
	
}

std::string Client::GenerateIdentifyPacket() {
	rapidjson::Document document;
	
	rapidjson::Pointer("/op").Set(document, 2);
	rapidjson::Pointer("/d/token").Set(document, this->token.c_str());
	
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

void Client::Run() {
	using namespace std;

	websocket.on_message = [this](shared_ptr<WssClient::Connection> connection, shared_ptr<WssClient::InMessage> in_message) {
		std::string response = in_message->string(); // string() can only be called once! see sws/client_ws.hpp for why
		cout << "Client: Message received: \"" << response << "\"" << endl;
		
	
		rapidjson::Document document;
		document.Parse(response.c_str());
		int opcode = document["op"].GetInt();
		
		if(opcode == 10) { // HELLO
			rapidjson::Value eventData = document["d"].GetObject();
			heartbeatInterval = eventData["heartbeat_interval"].GetInt();

			heartbeatTimer = new asio::steady_timer(*websocket.io_service);
			heartbeatTimer->expires_from_now(std::chrono::milliseconds(heartbeatInterval - 2000));
			heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this, std::placeholders::_1));

			std::cout << "Received HELLO (opcode 10) packet. Heartbeat interval: " << heartbeatInterval << std::endl;
			
			std::string identify_packet = GenerateIdentifyPacket();
			
			connection->send(identify_packet);
			
		}
		
	};

	websocket.on_open = [this](shared_ptr<WssClient::Connection> connection) {
		cout << "Client: Opened connection to " << connection->remote_endpoint_address() << endl;
		this->connection = connection;
	};

	websocket.on_close = [this](shared_ptr<WssClient::Connection> /*connection*/, int status, const string & /*reason*/) {
		cout << "Client: Closed connection with status code " << status << endl;
		heartbeatTimer->cancel();
	};

	websocket.on_error = [](shared_ptr<WssClient::Connection> /*connection*/, const SimpleWeb::error_code &ec) {
		cout << "Client: Error: " << ec << ", error message: " << ec.message() << endl;
	};
	
	websocket.start();
}
