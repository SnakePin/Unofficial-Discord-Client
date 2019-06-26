#pragma once

#include <discord/snowflake.hpp>
#include <discord/embed.hpp>
#include <discord/reaction.hpp>

#include <discord/user.hpp>

#include <rapidjson/document.h>

#include <optional>

namespace Discord {
	
	struct Message {
		// https://discordapp.com/developers/docs/resources/channel#message-object
		Snowflake id;
		std::optional<Snowflake> channelID;
		std::optional<Snowflake> guildID;
		std::optional<Snowflake> nonce;
		std::optional<int32_t> type;

		std::optional<User> author;

		std::optional<std::string> content;
		std::optional<std::string> timestamp;
		std::optional<std::string> editedTimestamp;
		std::optional<bool> tts;
		std::optional<bool> mentionEveryone;

		// User mentions[]
		std::vector<Snowflake> mentionedroleIDs;

		// attachments[]
		std::vector<Embed> embeds;
		std::vector<Reaction> reactions;

		std::optional<bool> pinned;
		std::optional<Snowflake> webhookID;

		// Two *Rich Presence* fields were not implemented
		// MessageActivityObject activity;
		// MessageApplicationObject application;

		static Message LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

	struct MessagePacket {
		// https://discordapp.com/developers/docs/resources/channel#create-message-params
		std::string content;
		bool tts;
		
	};

}