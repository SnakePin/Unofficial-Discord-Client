#pragma once

#include "discord/snowflake.hpp"
#include "discord/emoji.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

namespace Discord {
	
	struct Reaction : Deserializable_Serializable_Class<Reaction> {
		// https://discordapp.com/developers/docs/resources/channel#reaction-object

		int32_t count;
		bool me;
		Emoji emoji;

		static Reaction LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

}