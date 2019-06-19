#include "discord/discord.hpp"

#include "sws/client_wss.hpp"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>

#include <memory>

using namespace Discord;

Client::Client(std::string token, bool bot) : token(token), bot(bot),
    websocket("gateway.discord.gg/?v=6&encoding=json", false) {
	
}

std::string Client::generate_identify_packet() {
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

void Client::SendHeartbeatAndResetTimer() {
	if(heartbeatTimer == nullptr) {
		return;
	}
	std::cout << "Heartbeat!";
	
	heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this));
	ioService.run();
}

void Client::run() {
	using namespace std;

	websocket.on_message = [this](shared_ptr<WssClient::Connection> connection, shared_ptr<WssClient::InMessage> in_message) {
		std::string response = in_message->string(); // string() can only be called once! see sws/client_ws.hpp for why
		cout << "Client: Message received: \"" << response << "\"" << endl;
		
	
		rapidjson::Document document;
		document.Parse(response.c_str());
		int opcode = document["op"].GetInt();
		
		if(opcode == 10) { // HELLO
			rapidjson::Value eventData = document["d"].GetObject();
			int heartbeat_interval = eventData["heartbeat_interval"].GetInt();

			// -5 to counter heartbeat timeouts caused by network delay
			heartbeatTimer = new asio::deadline_timer(ioService, boost::posix_time::milliseconds(5000));
			heartbeatTimer->async_wait(std::bind(&Client::SendHeartbeatAndResetTimer, this));
			ioService.run();

			std::cout << "Received HELLO (opcode 10) packet. Heartbeat interval: " << heartbeat_interval << std::endl;
			
			std::string identify_packet = generate_identify_packet();
			
			connection->send(identify_packet);
			
			// TODO respond to heartbeats (opcode 11)
			// TODO send heartbeat every heartbeat_interval milliseconds
		}
		
		// cout << "Client: Sending close connection" << endl;
		// connection->send_close(1000);
	};

	websocket.on_open = [](shared_ptr<WssClient::Connection> connection) {
		cout << "Client: Opened connection" << endl;

		string out_message("Hello");

		// cout << "Client: Sending message: \"" << out_message << "\"" << endl;

		// connection->send(out_message);
	};

	websocket.on_close = [this](shared_ptr<WssClient::Connection> /*connection*/, int status, const string & /*reason*/) {
		cout << "Client: Closed connection with status code " << status << endl;

		delete heartbeatTimer;
		heartbeatTimer = nullptr;
	};

	// See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
	websocket.on_error = [](shared_ptr<WssClient::Connection> /*connection*/, const SimpleWeb::error_code &ec) {
		cout << "Client: Error: " << ec << ", error message: " << ec.message() << endl;
	};
	
	websocket.start();
}
