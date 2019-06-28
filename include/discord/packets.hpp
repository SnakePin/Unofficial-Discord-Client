#pragma once

#include <discord/user.hpp>
#include <discord/emoji.hpp>
#include <discord/guild.hpp>
#include <discord/channel.hpp>

#include <vector>
#include <optional>

namespace Discord {

	struct ReadyPacket {
		int32_t version;
		User user;

		std::vector<Channel> privateChannels;
		std::vector<Guild> guilds;
		std::string sessionID;

		static ReadyPacket LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

	struct TypingStartPacket {
		Snowflake userID;
		std::optional<Snowflake> guildID;
		Snowflake channelID;

		std::optional<Member> member;

		uint64_t timestamp;
		
		static TypingStartPacket LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

	// Used for MESSAGE_REACTION_ADD and MESSAGE_REACTION_REMOVE events.
	// Not used for MESSAGE_REACTION_REMOVE_ALL.
	// https://discordapp.com/developers/docs/topics/gateway#message-reaction-add
	struct MessageReactionPacket {
		Snowflake userID;
		Snowflake channelID;
		Snowflake messageID;
		std::optional<Snowflake> guildID;
		Emoji emoji;

		static MessageReactionPacket LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}