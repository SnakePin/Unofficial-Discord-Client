#pragma once

#include <string>
#include <vector>
#include <asio.hpp>

#include <discord/message.hpp>
#include <discord/guild.hpp>
#include <discord/token.hpp>
#include <discord/snowflake.hpp>
#include <discord/packets.hpp>

#include <sws/client_wss.hpp>

#include <rapidjson/document.h>

namespace Discord {
	
	using WssClient = SimpleWeb::SocketClient<SimpleWeb::WSS>;
	
	class Client {
	public:
		AuthToken token;
		std::string sessionID;
		int heartbeatInterval;
		WssClient websocket;

		uint64_t sequenceNumber;


		Client(std::string token, AuthTokenType tokenType);
		
		// Generate and send an IDENTIFY packet
		void SendIdentify();

		// Generate and send a RESUME packet
		void SendResume(std::string sessionID, uint32_t sequenceNumber);

		// Start the websocket and event loop.
		// This will block until the event loop is stopped (websocket.stop()).
		void Run();

		// Websocket Gateway Events
		// OnHelloPacket's default behavior is to call SendIdentify().
		virtual void OnHelloPacket();
		virtual void OnReadyPacket(ReadyPacket packet) = 0;
		virtual void OnGuildCreate(Guild guild) = 0;
		virtual void OnMessageCreate(Message m) = 0;
		virtual void OnTypingStart(TypingStartPacket p) = 0;
		virtual void OnMessageReactionAdd(MessageReactionPacket p) = 0;
		virtual void OnMessageReactionRemove(MessageReactionPacket p) = 0;

		class HTTP_API_CLASS
		{
		public:
			std::string userAgent;
			HTTP_API_CLASS(const Client& clientObj);
			HTTP_API_CLASS(const AuthToken _token);

			void StartTyping(const Snowflake &channelID);
			void SendMessage(const Snowflake &channelID, MessagePacket messageToSend);

			const AuthToken token;
		};
		HTTP_API_CLASS httpAPI;
	private:
	
		// Gets set every time the websocket opens a new connection.
		std::shared_ptr<WssClient::Connection> connection;

		// Used with SendHeartbeatAndResetTimer.
		asio::steady_timer *heartbeatTimer;


		// Gateway Packet Processing
		// These functions call the gateway event methods above.
		void ProcessHello(rapidjson::Document &document);
		void ProcessReady(rapidjson::Document &document);
		void ProcessGuildCreate(rapidjson::Document &document);
		void ProcessMessageCreate(rapidjson::Document &document);

		void SendHeartbeatAndResetTimer(const asio::error_code& error);

		// Generates an IDENTIFY packet.
		// https://discordapp.com/developers/docs/topics/gateway#identify
		std::string GenerateIdentifyPacket();

		// Generates a RESUME packet using the specified sessionID and sequenceNumber.
		// https://discordapp.com/developers/docs/topics/gateway#resume
		std::string GenerateResumePacket(std::string sessionID, uint32_t sequenceNumber);


	public:
		// Switches view to a Guild/Channel
		void OpenGuildChannel(const Snowflake &guild, const Snowflake &channel);

	};
}
