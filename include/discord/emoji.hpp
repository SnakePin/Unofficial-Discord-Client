#pragma once

#include "discord/snowflake.hpp"

#include <rapidjson/document.h>

#include <optional>

namespace Discord {

	struct Emoji {
		// https://discordapp.com/developers/docs/resources/emoji#emoji-object
		
		Snowflake id;
		std::string name;

		// roles[]
		// User creator;

		std::optional<bool> requireColons;
		std::optional<bool> managed;
		std::optional<bool> animated;

		static Emoji LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}