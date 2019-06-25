#pragma once

#include <string>
#include <vector>
#include <asio.hpp>

#include <discord/message.hpp>
#include <discord/guild.hpp>
#include <discord/token.hpp>
#include <sws/client_wss.hpp>

#include <rapidjson/document.h>

namespace Discord {
	
	using WssClient = SimpleWeb::SocketClient<SimpleWeb::WSS>;
	
	class Client {
	public:
		AuthToken token;
		int heartbeatInterval;
		WssClient websocket;

		Client(std::string token, AuthTokenType tokenType);
		
		void Run();
		
		virtual void OnGuildCreate(Guild guild) = 0;

		class HTTP_API_CLASS
		{
		public:
			HTTP_API_CLASS(const Client& clientObj);
			HTTP_API_CLASS(const AuthToken _token);

			// TODO: Use "Discord::Channel channel" instead of "std::string channelID" here when Discord::Channel is implemented.
			void StartTyping(std::string channelID);
			void SendMessage(std::string channelID, Discord::MessagePacket messageToSend);

			const AuthToken token;
		};
		HTTP_API_CLASS httpAPI;
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
