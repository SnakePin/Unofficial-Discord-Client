#pragma once

#include <discord/snowflake.hpp>

namespace Discord {

	struct Role {
		// https://discordapp.com/developers/docs/resources/role#role-object
		
		Snowflake id;
		std::string name;

		uint32_t color;

		bool hoist;
		int32_t position;
		int32_t permissions;
		bool managed;
		bool mentionable;
	};

}