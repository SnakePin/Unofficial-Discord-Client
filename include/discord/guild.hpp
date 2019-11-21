#pragma once

#include "discord/snowflake.hpp"
#include "discord/emoji.hpp"
#include "discord/role.hpp"
#include "discord/channel.hpp"
#include "discord/member.hpp"

#include <vector>
#include <string>

#include <optional>

#include <rapidjson/document.h>

namespace Discord {

	struct Guild {
		// https://discordapp.com/developers/docs/resources/guild#guild-object

		Snowflake id;
		std::string name;
		std::string splashHash;
		std::string iconHash;

		Snowflake ownerID;
		std::optional<uint32_t> permissions;
		
		std::string region;

		Snowflake afkChannelID;
		int32_t afkTimeout;
		
		std::optional<bool> embedable;
		std::optional<Snowflake> embedChannelID;

		uint8_t verificationLevel;
		uint8_t defaultMessageNotificationsLevel;
		uint8_t explicitContentFilterLevel;
		uint8_t mfaLevel;

		std::vector<Role> roles;
		std::vector<Emoji> emoji;

		std::vector<std::string> features;

		Snowflake applicationID;
		
		std::optional<bool> widgetEnabled;
		std::optional<Snowflake> widgetChannelID;

		Snowflake systemChannelID;

		std::optional<std::string> joinedAt;

		std::optional<bool> large;
		std::optional<bool> unavailable;
		std::optional<int32_t> memberCount;

		// voicestates[]

		std::vector<Member> members;
		std::vector<Channel> channels;

		int32_t maxPresences;
		int32_t maxMembers;
		int32_t premiumTier;

		std::string description;
		std::string bannerHash;

		static Guild LoadFrom(rapidjson::Document &doc, std::string pointer = "");

	};

}
