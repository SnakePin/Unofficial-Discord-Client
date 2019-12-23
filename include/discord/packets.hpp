#pragma once

#include "discord/user.hpp"
#include "discord/emoji.hpp"
#include "discord/guild.hpp"
#include "discord/channel.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

#include <vector>
#include <optional>
#include <variant>
#include <functional>

namespace Discord
{

struct GuildMemberListGroup : JsonDeSerializable
{
	std::string id; // NOT a Snowflake
	int32_t count;
	
	bool LoadFrom(rapidjson::Document &doc, std::string pointer = "");
};

struct GuildMemberListUpdateOperation : JsonDeSerializable
{
	std::pair<int32_t, int32_t> range;
	std::string op;
	std::vector<std::variant<GuildMemberListGroup, Member>> items;

	bool LoadFrom(rapidjson::Document &doc, std::string pointer = "");
};

struct GuildMemberListUpdatePacket : JsonDeSerializable
{
	std::vector<GuildMemberListUpdateOperation> operations;

	std::string id; // NOT a Snowflake
	Snowflake guildID;
	std::vector<GuildMemberListGroup> groups;

	bool LoadFrom(rapidjson::Document &doc, std::string pointer = "");
};

struct ReadyPacket : JsonDeSerializable
{
	int32_t version;
	User user;

	std::vector<Channel> privateChannels;
	std::vector<Guild> guilds;
	std::string sessionID;
	// Used for sending Discord's "science" requests.
	std::string analyticsToken; // note for future: This is one of the many things which do not exist for Bot accounts.

	bool LoadFrom(rapidjson::Document &doc, std::string pointer = "");
};

struct TypingStartPacket : JsonDeSerializable
{
	Snowflake userID;
	std::optional<Snowflake> guildID;
	Snowflake channelID;

	std::optional<Member> member;

	uint64_t timestamp;

	bool LoadFrom(rapidjson::Document &doc, std::string pointer = "");
};

// Used for MESSAGE_REACTION_ADD and MESSAGE_REACTION_REMOVE events.
// Not used for MESSAGE_REACTION_REMOVE_ALL.
// https://discordapp.com/developers/docs/topics/gateway#message-reaction-add
struct MessageReactionPacket : JsonDeSerializable
{
	Snowflake userID;
	Snowflake channelID;
	Snowflake messageID;
	std::optional<Snowflake> guildID;
	Emoji emoji;

	bool LoadFrom(rapidjson::Document &doc, std::string pointer = "");
};

} // namespace Discord