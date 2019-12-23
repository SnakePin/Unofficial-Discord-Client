#pragma once

#include "discord/snowflake.hpp"
#include "discord/embed.hpp"
#include "discord/reaction.hpp"
#include "discord/user.hpp"
#include "discord/attachment.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

#include <rapidjson/document.h>

#include <optional>

namespace Discord {
	
	struct Message : JsonDeSerializable {
		// https://discordapp.com/developers/docs/resources/channel#message-object
		Snowflake id;
		Snowflake channelID;
		std::optional<Snowflake> guildID;
		Snowflake nonce;
		int32_t type;

		User author;

		std::string content;
		std::string timestamp;
		std::string editedTimestamp;
		bool tts;
		bool mentionEveryone;

		std::vector<User> mentions;
		std::vector<Snowflake> mentionedroleIDs;

		std::vector<Attachment> attachments;
		std::vector<Embed> embeds;
		std::vector<Reaction> reactions;

		bool pinned;
		std::optional<Snowflake> webhookID;

		// Two *Rich Presence* fields were not implemented
		// MessageActivityObject activity;
		// MessageApplicationObject application;

		bool LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

	struct CreateMessageParam {
		// https://discordapp.com/developers/docs/resources/channel#create-message-params
		std::string content;
		bool tts;
		
	};

}