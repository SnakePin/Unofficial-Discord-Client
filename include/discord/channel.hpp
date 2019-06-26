#pragma once

#include <discord/snowflake.hpp>

#include <rapidjson/document.h>

#include <optional>

namespace Discord {
	
	struct Channel {
		// https://discordapp.com/developers/docs/resources/channel#channel-object

		Snowflake id;
		int32_t type;
		std::optional<std::string> name;
		std::optional<std::string> topic;
		std::optional<bool> nsfw;
		std::optional<std::string> iconHash;
		
		std::optional<Snowflake> guildID;
		std::optional<Snowflake> ownerID;
		std::optional<Snowflake> applicationID;
		std::optional<Snowflake> parentCategoryID;
		std::optional<Snowflake> lastMessageID;
		
		std::optional<int32_t> position;
		std::optional<uint32_t> permissionOverwrites;

		std::optional<int32_t> bitrate;
		std::optional<int32_t> userLimit;
		std::optional<int32_t> rateLimitPerUser;

		// User[] recipients

		std::optional<std::string> lastPinTimestamp;

		static Channel LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}