#pragma once

#include "discord/snowflake.hpp"
#include "discord/jsonDeSerializable_class.hpp"

#include <rapidjson/document.h>
#include <optional>
#include <string>

namespace Discord {

	struct Emoji : JsonDeSerializable {
		// https://discordapp.com/developers/docs/resources/emoji#emoji-object

		std::optional<Snowflake> id;
		std::optional<std::string> name;

		// roles[]
		// User creator;

		std::optional<bool> requireColons;
		std::optional<bool> managed;
		std::optional<bool> animated;

		bool LoadFrom(rapidjson::Value& object);
	};

}