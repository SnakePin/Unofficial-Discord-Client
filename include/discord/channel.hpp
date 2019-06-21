#pragma once

#include <discord/snowflake.hpp>

#include <rapidjson/document.h>

namespace Discord {
	
	struct Channel : public ISnowflake {
		// https://discordapp.com/developers/docs/resources/channel#channel-object

		int32_t type;
		std::string name;
		std::string topic;
		bool nsfw;
		std::string iconHash;
		
		Snowflake guildID;
		Snowflake ownerID;
		Snowflake applicationID;
		Snowflake parentCategoryID;
		Snowflake lastMessageID;
		
		int32_t position;
		uint32_t permissionOverwrites;

		int32_t bitrate;
		int32_t userLimit;
		int32_t rateLimitPerUser;

		// User[] recipients

		std::string lastPinTimestamp;

		static Channel LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}