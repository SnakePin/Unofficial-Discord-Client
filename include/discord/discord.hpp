#pragma once

#include <string>
#include <vector>
#include <asio.hpp>

#include <discord/message.hpp>
#include <sws/client_wss.hpp>

#include <rapidjson/document.h>

namespace Discord {
	
	using WssClient = SimpleWeb::SocketClient<SimpleWeb::WSS>;
	
	class Listener {
		virtual void on_message_receive(Discord::Message message) = 0;
	};

	
	class Client {
	public:
		std::vector<Listener*> listeners;
		
		std::string token;
		bool bot;
		int heartbeatInterval;
		WssClient websocket;
				
		Client(std::string token, bool bot = true);
		
		void Run();
		
		inline void AddListener(Listener *listener) { listeners.push_back(listener); }

	private:
	
		std::shared_ptr<WssClient::Connection> connection;
		asio::steady_timer *heartbeatTimer;
		unsigned int heartbeatSequenceNumber;
		uint64_t sequenceNumber;

		// Gateway Packet Processing
		void ProcessHello(rapidjson::Document &document);
		void ProcessReady(rapidjson::Document &document);
		void ProcessGuildCreate(rapidjson::Document &document);

		void SendHeartbeatAndResetTimer(const asio::error_code& error);
		std::string GenerateIdentifyPacket();

	};
}
