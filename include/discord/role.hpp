#pragma once

#include <discord/snowflake.hpp>

#include <vector>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

namespace Discord {

	struct Role : public ISnowflake {
		// https://discordapp.com/developers/docs/resources/role#role-object
		
		std::string name;

		uint32_t color;

		bool hoist;
		int32_t position;
		int32_t permissions;
		bool managed;
		bool mentionable;

		static Role LoadFrom(rapidjson::Document &doc, std::string pointer);
	};
}