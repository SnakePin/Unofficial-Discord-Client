#pragma once

#include <discord/discord.hpp>
#include <discord/message.hpp>
#include <discord/guild.hpp>
#include <discord/packets.hpp>
#include <discord/snowflake.hpp>

#include <asio.hpp>
#include <asio/thread_pool.hpp>

#include <vector>

class MyClient : public Discord::Client {
public:

	std::shared_ptr<asio::thread_pool> pool; // user created threadpool for making HTTP requests
	std::vector<Discord::Guild> guilds;
	std::vector<Discord::Channel> privateChannels;
	std::time_t lastSessionUpdateTime;

	MyClient(std::string& token, Discord::AuthTokenType tokenType);

	// Writes the session ID, sequence number, and update time to session.json.
	void UpdateSessionJson();

	void OnHelloPacket();

	void OnReadyPacket(Discord::ReadyPacket packet);

	void OnGuildCreate(Discord::Guild g);

	void OnGuildMemberListUpdate(Discord::GuildMemberListUpdatePacket packet);

	void OnTypingStart(Discord::TypingStartPacket packet);

	void OnMessageCreate(Discord::Message m);

	void OnMessageReactionAdd(Discord::MessageReactionPacket p);

	void OnMessageReactionRemove(Discord::MessageReactionPacket p);

	// Reads the session_id and sequence number from session.json and sends a RESUME packet with the read information.
	// This will do nothing if the session ID is too old.
	void LoadAndSendResume();
};