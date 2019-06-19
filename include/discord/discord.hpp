#pragma once

#include <string>
#include <vector>

#include "discord/message.hpp"
#include "sws/client_wss.hpp"

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
		
		WssClient websocket;
				
		Client(std::string token, bool bot = true);
		
		void run();
		
		
		
		inline void addListener(Listener *listener) { listeners.push_back(listener); }
	};
}
