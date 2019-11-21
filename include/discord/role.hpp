#pragma once

#include "discord/snowflake.hpp"

#include <vector>
#include <string>
#include <optional>

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

namespace Discord {

	struct Role {
		// https://discordapp.com/developers/docs/resources/role#role-object
		
		Snowflake id;
		std::string name;

		int32_t color;

		bool hoist;
		int32_t position;
		int32_t permissions;
		bool managed;
		bool mentionable;

		static Role LoadFrom(rapidjson::Document &doc, std::string pointer = "");
	};
}