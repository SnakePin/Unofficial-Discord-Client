#pragma once

#include <discord/snowflake.hpp>

namespace Discord {
	
	struct Channel {
		// https://discordapp.com/developers/docs/resources/channel#channel-object

		Snowflake id;
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
	};

}