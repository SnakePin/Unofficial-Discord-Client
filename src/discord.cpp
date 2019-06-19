#include "discord/discord.hpp"

#include "sws/client_wss.hpp"

#include <rapidjson/document.h>

#include <memory>

using namespace Discord;

Client::Client(std::string token, bool bot) : token(token), bot(bot),
    websocket("gateway.discord.gg/?v=6&encoding=json", false) {
	
}

void Client::run() {
	using namespace std;
	
	websocket.on_message = [](shared_ptr<WssClient::Connection> connection, shared_ptr<WssClient::InMessage> in_message) {
		std::string response = in_message->string(); // string() can only be called once! see sws/client_ws.hpp for why
		cout << "Client: Message received: \"" << response << "\"" << endl;
		
	
		rapidjson::Document document;
		document.Parse(response.c_str());
		int opcode = document["op"].GetInt();
		rapidjson::Value eventData = document["d"].GetObject();
		
		
		if(opcode == 10) { // HELLO
			int heartbeat_interval = eventData["heartbeat_interval"].GetInt();
			std::cout << "Received HELLO (opcode 10) packet. Heartbeat interval: " << heartbeat_interval << std::endl;
			
			// TODO generate and send an IDENTIFY response
			// TODO respond to heartbeats (opcode 11)
			// TODO send heartbeat every heartbeat_interval milliseconds
		}
		
		cout << "Client: Sending close connection" << endl;
		connection->send_close(1000);
	};

	websocket.on_open = [](shared_ptr<WssClient::Connection> connection) {
		cout << "Client: Opened connection" << endl;

		string out_message("Hello");
		// cout << "Client: Sending message: \"" << out_message << "\"" << endl;

		// connection->send(out_message);
	};

	websocket.on_close = [](shared_ptr<WssClient::Connection> /*connection*/, int status, const string & /*reason*/) {
		cout << "Client: Closed connection with status code " << status << endl;
	};

	// See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
	websocket.on_error = [](shared_ptr<WssClient::Connection> /*connection*/, const SimpleWeb::error_code &ec) {
		cout << "Client: Error: " << ec << ", error message: " << ec.message() << endl;
	};
	
	websocket.start();
}
