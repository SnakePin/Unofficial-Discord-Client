#pragma once

#include <discord/snowflake.hpp>

#include <vector>
#include <string>
#include <optional>

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

namespace Discord {

	struct Role {
		// https://discordapp.com/developers/docs/resources/role#role-object
		
		Snowflake id;
		std::optional<std::string> name;

		std::optional<int32_t> color;

		std::optional<bool> hoist;
		std::optional<int32_t> position;
		std::optional<int32_t> permissions;
		std::optional<bool> managed;
		std::optional<bool> mentionable;

		static Role LoadFrom(rapidjson::Document &doc, std::string pointer);
	};
}