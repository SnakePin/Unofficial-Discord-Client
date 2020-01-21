#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "discord/message.hpp"
#include "discord/guild.hpp"
#include "discord/token.hpp"
#include "discord/snowflake.hpp"
#include "discord/packets.hpp"
#include "discord/InternalUtils.hpp"

#include <cpprest/http_client.h>
#include <cpprest/ws_client.h>

#include <rapidjson/document.h>

namespace Discord {
	using WssClient = web::websockets::client::websocket_callback_client;

	const utility::string_t DefaultUserAgentString = U("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/75.0.3770.90 Chrome/75.0.3770.90 Safari/537.36");
	const utility::string_t DefaultGatewayURL = U("wss://gateway.discord.gg/?v=6&encoding=json");
	const utility::string_t DefaultAPIURL = U("https://discordapp.com/api/v6/");

	class Client {
	public:
		AuthToken token;
		std::string sessionID;
		int heartbeatInterval;
		WssClient websocket;
		uint64_t sequenceNumber = 0;

		//User agent string to use in gateway connection
		//TODO: give user ability to change this value
		const utility::string_t userAgent;

		Client(const std::string& token, AuthTokenType tokenType);
		Client(const Client& other);
		~Client();

		//Warning: This function will stop the client.
		Client& operator=(const Client& other);

		// Generate and send an IDENTIFY packet
		void SendIdentify();

		// Generate and send a RESUME packet
		void SendResume(std::string& sessionID, uint32_t sequenceNumber);

		// Start the websocket and event loop.
		// This will block until the event loop is stopped with Stop().
		void Run();

		// Stop the event loop and websocket
		void Stop();

		bool IsRunning();

		// Websocket Gateway Events
		// Note: These virtual functions are not pure virtual functions, we have declarations for them, they are meant to be overrided if user wants to
		// OnHelloPacket's default behavior is to call SendIdentify().
		virtual void OnHelloPacket();
		virtual void OnResumeFail();
		virtual void OnResumeSuccess();
		virtual void OnReadyPacket(ReadyPacket packet);
		virtual void OnGuildCreate(Guild guild);
		virtual void OnGuildMemberListUpdate(GuildMemberListUpdatePacket packet);
		virtual void OnMessageCreate(Message m);
		virtual void OnTypingStart(TypingStartPacket p);
		virtual void OnMessageReactionAdd(MessageReactionPacket p);
		virtual void OnMessageReactionRemove(MessageReactionPacket p);
		virtual void OnWSSDisconnect(std::error_code errorCode, std::string reason);
		virtual void OnWSSConnect();
		virtual void OnReconnectPacket();
		virtual void OnPostStop();

		// HTTP API instance and HTTP API class declaration
		class HTTP_API_CLASS
		{
		public:
			HTTP_API_CLASS(const Client& clientObj);
			HTTP_API_CLASS(const AuthToken _token);

			bool TriggerTypingIndicator(const Snowflake& channelID);
			bool CreateMessage(const Snowflake& channelID, CreateMessageParam messageToSend);
			bool DeleteMessage(const Snowflake& channelID, const Snowflake& messageID);
			bool GetChannel(const Snowflake& channelID, Channel& channel);
			bool GetChannelMessage(const Snowflake& channelID, const Snowflake& messageID, Message& message);
			bool GetPinnedMessages(const Snowflake& channelID, std::vector<Message>& messages);
			bool UnpinChannelMessage(const Snowflake& channelID, const Snowflake& messageID);

			bool GetCurrentUser(User& user);
			bool GetUserByID(User& user, const Snowflake& userID);

			// Requests the (default: 50) most recent messages from a given channel, and
			// pushes them into the vector. Returns true if the request was successful.
			// Success does not necessarily mean that any new messages were received, however.
			bool GetMessagesInto(const Snowflake& channelID, std::vector<Message>& messages, int count = 50);

			// TODO replace with an enum
			// TODO implement
			// Sends a PATCH to https://discordapp.com/api/v6/users/@me/settings
			// whose body is the JSON object {"status":"idle"} or similar.
			bool UpdatePresenceStatusSetting(std::string& status);

			const AuthToken token;

			//User agent string to use in HTTP API connections
			//TODO: give user ability to change this value
			utility::string_t userAgent;

		private:

			web::http::client::http_client client{ DefaultAPIURL };
		} httpAPI;

	private:
		std::atomic<bool> isRunning = false;

		InternalUtils::SimpleAsyncTimedEvent heartbeatTimer;

		// Gateway Packet Processing
		// These functions call the gateway event methods above.
		void ProcessHello(rapidjson::Document& document);
		void ProcessReady(rapidjson::Document& document);
		void ProcessDispatch(rapidjson::Document& document);
		void ProcessReconnectPacket(rapidjson::Document& document);

		void SendHeartbeatAndResetTimer();

		// Generates an IDENTIFY packet.
		// https://discordapp.com/developers/docs/topics/gateway#identify
		std::string GenerateIdentifyPacket(bool compress = false);

		// Generates a RESUME packet using the specified sessionID and sequenceNumber.
		// https://discordapp.com/developers/docs/topics/gateway#resume
		std::string GenerateResumePacket(std::string& sessionID, uint32_t sequenceNumber);

		std::string GenerateGuildChannelViewPacket(const Snowflake& guild, const Snowflake& channel);

	public:

		// Sends an OP 14 signal.
		// This tells Discord what channel we are 'looking' at.
		// OP 14 (see scripts/outbound_packets/op14.json for what this looks like)
		void OpenGuildChannelView(const Snowflake& guild, const Snowflake& channel);

		// Sends an OP 13 signal.
		// See OpenGuildChannelView (OP 14). Does the same, but for DM channels.
		// OP 13 (see scripts/outbound_packets/op13.json for what this looks like)
		void OpenPrivateChannelView(const Snowflake& channel);

		// Sends an OP 3 signal.
		// Distinct from HTTP API's UpdatePresenceStatusSetting, because it only temporarily
		// sets the status to this. Used when the Client program has detected no activity
		// for 10 minutes, and decides that the user is now idle.
		void UpdatePresence(std::string& status);

	};
}
