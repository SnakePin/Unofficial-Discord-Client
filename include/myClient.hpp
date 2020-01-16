#pragma once

#include <discord/discord.hpp>
#include <discord/message.hpp>
#include <discord/guild.hpp>
#include <discord/packets.hpp>
#include <discord/snowflake.hpp>

#include <asio.hpp>
#include <asio/thread_pool.hpp>

#include <vector>
#include <mutex>
#include <atomic>

class MyClient : public Discord::Client {
public:

	std::shared_ptr<asio::thread_pool> pool; // user created threadpool for making HTTP requests
	std::mutex guildsVectorMutex;
	std::vector<Discord::Guild> guilds;
	std::mutex privateChannelsVectorMutex;
	std::vector<Discord::Channel> privateChannels;
	std::time_t lastSessionUpdateTime;
	std::atomic<bool> isIdentified = false;

	MyClient(std::string& token, Discord::AuthTokenType tokenType);

	MyClient(const MyClient& other)
		: Discord::Client(other) // dispatch to base copy constructor
	{
		pool = other.pool;
		guilds = other.guilds;
		privateChannels = other.privateChannels;
		privateChannels = other.privateChannels;
		lastSessionUpdateTime = other.lastSessionUpdateTime;
	}

	MyClient& operator=(const MyClient& other)
	{
		Discord::Client::operator=(other);
		pool = other.pool;
		guilds = other.guilds;
		privateChannels = other.privateChannels;
		privateChannels = other.privateChannels;
		lastSessionUpdateTime = other.lastSessionUpdateTime;

		return *this;
	}

	void StopAndSaveSession();

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
	void OnStop();
	void OnResumeSuccess();

	// Reads the session_id and sequence number from session.json and sends a RESUME packet with the read information.
	// This will do nothing if the session ID is too old.
	void LoadAndSendResume();
};