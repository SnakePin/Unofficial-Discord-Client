#pragma once

#include "discord/snowflake.hpp"
#include "discord/emoji.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

namespace Discord {
	
	struct Reaction : JsonDeSerializable {
		// https://discordapp.com/developers/docs/resources/channel#reaction-object

		int32_t count;
		bool me;
		Emoji emoji;

		bool LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

}