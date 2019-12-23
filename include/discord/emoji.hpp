#pragma once

#include "discord/snowflake.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

#include <rapidjson/document.h>
#include <optional>

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

		bool LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

}