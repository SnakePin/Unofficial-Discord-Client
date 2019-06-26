#pragma once

#include <discord/snowflake.hpp>
#include <discord/emoji.hpp>
#include <discord/role.hpp>
#include <discord/channel.hpp>
#include <discord/member.hpp>

#include <vector>
#include <string>

#include <optional>

#include <rapidjson/document.h>

namespace Discord {

	struct Guild {
		// https://discordapp.com/developers/docs/resources/guild#guild-object

		Snowflake id;
		std::optional<std::string> name;
		std::optional<std::string> splashHash;
		std::optional<std::string> iconHash;

		std::optional<Snowflake> ownerID;
		std::optional<uint32_t> permissions;
		
		std::optional<std::string> region;

		std::optional<Snowflake> afkChannelID;
		std::optional<int32_t> afkTimeout;
		
		std::optional<bool> embedable;
		std::optional<Snowflake> embedChannelID;

		std::optional<uint8_t> verificationLevel;
		std::optional<uint8_t> defaultMessageNotificationsLevel;
		std::optional<uint8_t> explicitContentFilterLevel;
		std::optional<uint8_t> mfaLevel;

		std::vector<Role> roles;
		std::vector<Emoji> emoji;

		std::vector<std::string> features;

		std::optional<Snowflake> applicationID;
		
		std::optional<bool> widgetEnabled;
		std::optional<Snowflake> widgetChannelID;

		std::optional<Snowflake> systemChannelID;

		std::optional<std::string> joinedAt;

		std::optional<bool> large;
		std::optional<bool> unavailable;
		std::optional<int32_t> memberCount;

		// voicestates[]

		std::vector<Member> members;
		std::vector<Channel> channels;

		std::optional<int32_t> maxPresences;
		std::optional<int32_t> maxMembers;
		std::optional<int32_t> premiumTier;

		std::optional<std::string> description;
		std::optional<std::string> bannerHash;

		static Guild LoadFrom(rapidjson::Document &doc, std::string pointer);

	};

}
