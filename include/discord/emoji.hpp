#pragma once

#include "discord/snowflake.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

#include <rapidjson/document.h>
#include <optional>

namespace Discord {

	struct Emoji : Deserializable_Serializable_Class<Emoji> {
		// https://discordapp.com/developers/docs/resources/emoji#emoji-object

		//TODO: make these two fields below nullable
		Snowflake id;
		std::string name;

		// roles[]
		// User creator;

		std::optional<bool> requireColons;
		std::optional<bool> managed;
		std::optional<bool> animated;

		static Emoji LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

}