#pragma once

#include <discord/snowflake.hpp>

#include <rapidjson/document.h>

namespace Discord {

	struct Emoji {
		// https://discordapp.com/developers/docs/resources/emoji#emoji-object
		
		Snowflake id;
		std::string name;

		// roles[]
		// User creator;

		bool requireColons;
		bool managed;
		bool animated;

		static Emoji LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}