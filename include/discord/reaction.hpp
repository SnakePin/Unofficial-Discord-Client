#pragma once

#include <discord/snowflake.hpp>

namespace Discord {
	
	struct Reaction {
		// https://discordapp.com/developers/docs/resources/channel#reaction-object

		int32_t count;
		bool me;
		Snowflake emojiID;
	};

}