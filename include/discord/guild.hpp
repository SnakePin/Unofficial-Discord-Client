#pragma once

#include "discord/snowflake.hpp"
#include "discord/emoji.hpp"
#include "discord/role.hpp"
#include "discord/channel.hpp"
#include "discord/member.hpp"
#include "discord/jsonDeSerializable_class.hpp"

#include <vector>
#include <string>

#include <optional>

#include <rapidjson/document.h>

namespace Discord {

	struct Guild : JsonDeSerializable {
		// https://discordapp.com/developers/docs/resources/guild#guild-object

		Snowflake id;
		std::string name;

		std::optional<std::string> iconHash;
		std::optional<std::string> splashHash;
		std::optional<std::string> discoverySplash;


		std::optional<bool> isCurrentUserOwner;
		Snowflake ownerID;

		std::optional<uint32_t> permissions_legacy;
		std::optional<std::string> permissions_new;
		
		std::string region;

		std::optional<Snowflake> afkChannelID;
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

		// presences[]

		std::optional<int32_t> maxPresences;
		std::optional<int32_t> maxMembers;
		int32_t premiumTier;
		std::optional<int32_t> premiumSubscriptionCount;

		std::optional<std::string> description;
		std::optional<std::string> bannerHash;

		std::string preferredLocale;
		std::optional<Snowflake> publicUpdatesChannelId;
		std::optional<int32_t> maxVideoChannelUsers;
		std::optional<int32_t> approximateMemberCount;
		std::optional<int32_t> approximatePresenceCount;

		bool LoadFrom(rapidjson::Value& object);
	};

}
