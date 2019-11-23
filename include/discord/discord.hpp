#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <asio.hpp>

#include "discord/message.hpp"
#include "discord/guild.hpp"
#include "discord/token.hpp"
#include "discord/snowflake.hpp"
#include "discord/packets.hpp"

#include <sws/client_wss.hpp>

#include <rapidjson/document.h>

namespace Discord {
	
	using WssClient = SimpleWeb::SocketClient<SimpleWeb::WSS>;

	constexpr std::string_view DefaultUserAgentString = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/75.0.3770.90 Chrome/75.0.3770.90 Safari/537.36";

	class Client {
	public:
		AuthToken token;
		std::string sessionID;
		int heartbeatInterval;
		WssClient websocket;

		uint64_t sequenceNumber;

		Client(std::string& token, AuthTokenType tokenType);
		
		// Generate and send an IDENTIFY packet
		void SendIdentify();

		// Generate and send a RESUME packet
		void SendResume(std::string& sessionID, uint32_t sequenceNumber);

		// Start the websocket and event loop.
		// This will block until the event loop is stopped (websocket.stop()).
		void Run();

		// Websocket Gateway Events
		// OnHelloPacket's default behavior is to call SendIdentify().
		virtual void OnHelloPacket() = 0;
		virtual void OnReadyPacket(ReadyPacket packet) = 0;
		virtual void OnGuildCreate(Guild guild) = 0;
		virtual void OnGuildMemberListUpdate(GuildMemberListUpdatePacket packet) = 0;
		virtual void OnMessageCreate(Message m) = 0;
		virtual void OnTypingStart(TypingStartPacket p) = 0;
		virtual void OnMessageReactionAdd(MessageReactionPacket p) = 0;
		virtual void OnMessageReactionRemove(MessageReactionPacket p) = 0;
		
		// HTTP API instance and HTTP API class declaration
		class HTTP_API_CLASS
		{
		public:
			HTTP_API_CLASS(const Client &clientObj);
			HTTP_API_CLASS(const AuthToken _token);
	
			bool TriggerTypingIndicator(const Snowflake &channelID);
			bool CreateMessage(const Snowflake &channelID, CreateMessageParam messageToSend);
			bool DeleteMessage(const Snowflake &channelID, const Snowflake &messageID);
			bool GetChannel(const Snowflake &channelID, Channel &channel);
			bool GetChannelMessage(const Snowflake &channelID, const Snowflake &messageID, Message &message);
			bool GetPinnedMessages(const Snowflake &channelID, std::vector<Message> &messages);
			bool UnpinChannelMessage(const Snowflake &channelID, const Snowflake &messageID);

			bool GetCurrentUser(User &user);
			bool GetUserByID(User &user, const Snowflake &userID);

			// Requests the (default: 50) most recent messages from a given channel, and
			// pushes them into the vector. Returns true if the request was successful.
			// Success does not necessarily mean that any new messages were received, however.
			bool GetMessagesInto(const Snowflake &channelID, std::vector<Message> &messages, int count = 50);

			// TODO replace with an enum
			// TODO implement
			// Sends a PATCH to https://discordapp.com/api/v6/users/@me/settings
			// whose body is the JSON object {"status":"idle"} or similar.
			bool UpdatePresenceStatusSetting(std::string& status);

			const AuthToken token;
			std::string userAgent;
		} httpAPI;

		// User agent string to use in gateway connection and in HTTP API instance
		std::string userAgent;
	private:
	
		// Gets set every time the websocket opens a new connection.
		std::shared_ptr<WssClient::Connection> connection;

		// Used with SendHeartbeatAndResetTimer.
		std::unique_ptr<asio::steady_timer> heartbeatTimer;

		std::shared_ptr<asio::io_context> io_context;

		// Gateway Packet Processing
		// These functions call the gateway event methods above.
		void ProcessHello(rapidjson::Document &document);
		void ProcessReady(rapidjson::Document &document);
		void ProcessGuildCreate(rapidjson::Document &document);
		void ProcessMessageCreate(rapidjson::Document &document);

		void SendHeartbeatAndResetTimer(const asio::error_code& error);

		// Generates an IDENTIFY packet.
		// https://discordapp.com/developers/docs/topics/gateway#identify
		std::string GenerateIdentifyPacket(bool compress = false);

		// Generates a RESUME packet using the specified sessionID and sequenceNumber.
		// https://discordapp.com/developers/docs/topics/gateway#resume
		std::string GenerateResumePacket(std::string& sessionID, uint32_t sequenceNumber);


		std::string GenerateGuildChannelViewPacket(const Snowflake &guild, const Snowflake &channel);

		void ScheduleNewWSSPacket(std::string_view out_message_str, const std::function<void(const std::error_code &)> &callback = nullptr, unsigned char fin_rsv_opcode = 129);

	public:

		// Sends an OP 14 signal.
		// This tells Discord what channel we are 'looking' at.
		// OP 14 (see scripts/outbound_packets/op14.json for what this looks like)
		void OpenGuildChannelView(const Snowflake &guild, const Snowflake &channel);

		// Sends an OP 13 signal.
		// See OpenGuildChannelView (OP 14). Does the same, but for DM channels.
		// OP 13 (see scripts/outbound_packets/op13.json for what this looks like)
		void OpenPrivateChannelView(const Snowflake &channel);

		// Sends an OP 3 signal.
		// Distinct from HTTP API's UpdatePresenceStatusSetting, because it only temporarily
		// sets the status to this. Used when the Client program has detected no activity
		// for 10 minutes, and decides that the user is now idle.
		void UpdatePresence(std::string& status);

	};
}
