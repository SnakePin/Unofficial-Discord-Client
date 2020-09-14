#pragma once

#include "discord/snowflake.hpp"
#include "discord/embed.hpp"
#include "discord/reaction.hpp"
#include "discord/user.hpp"
#include "discord/attachment.hpp"
#include "discord/jsonDeSerializable_class.hpp"

#include <rapidjson/document.h>

#include <optional>

namespace Discord {

	struct MessageActivityObject : JsonDeSerializable {
		enum class ActivityTypes : int32_t {
			JOIN = 1,
			SPECTATE = 2,
			LISTEN = 3,
			JOIN_REQUEST = 5
		} type;

		// https://discord.com/developers/docs/rich-presence/how-to#updating-presence-update-presence-payload-fields "id of the player's party, lobby, or group"
		std::optional<std::string> partyId;

		bool LoadFrom(rapidjson::Value& object);
	};

	struct MessageReferenceObject : JsonDeSerializable {
		//https://discord.com/developers/docs/resources/channel#message-object-message-reference-structure
		std::optional<Snowflake> messageId;
		std::optional<Snowflake> channelId;
		std::optional<Snowflake> guildId;

		bool LoadFrom(rapidjson::Value& object);
	};

	struct Message : JsonDeSerializable {
		// https://discordapp.com/developers/docs/resources/channel#message-object
		Snowflake id;
		Snowflake channelID;
		std::optional<Snowflake> guildID;
		Snowflake nonce;

		enum class MessageTypes : int32_t {
			DEFAULT = 0,
			RECIPIENT_ADD = 1,
			RECIPIENT_REMOVE = 2,
			CALL = 3,
			CHANNEL_NAME_CHANGE = 4,
			CHANNEL_ICON_CHANGE = 5,
			CHANNEL_PINNED_MESSAGE = 6,
			GUILD_MEMBER_JOIN = 7,
			USER_PREMIUM_GUILD_SUBSCRIPTION = 8,
			USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_1 = 9,
			USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_2 = 10,
			USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_3 = 11,
			CHANNEL_FOLLOW_ADD = 12,
			GUILD_DISCOVERY_DISQUALIFIED = 14,
			GUILD_DISCOVERY_REQUALIFIED = 15
		} type;

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

		std::optional<MessageActivityObject> activity;

		std::optional<MessageReferenceObject> messageReference;

		// One *Rich Presence* field was not implemented
		// MessageApplicationObject application;

		bool LoadFrom(rapidjson::Value& object);
	};

	struct CreateMessageParam {
		// https://discordapp.com/developers/docs/resources/channel#create-message-params
		std::string content;
		bool tts;
	};
}