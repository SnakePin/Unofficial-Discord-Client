#pragma once

#include "discord/snowflake.hpp"

#include <rapidjson/document.h>

#include <optional>

namespace Discord {

	struct Attachment {
		// https://discordapp.com/developers/docs/resources/channel#attachment-object
		Snowflake id;
		std::string filename;
		int32_t size; //In bytes
		std::string url;
		std::string proxy_url;
		std::optional<int32_t> height;
		std::optional<int32_t> width;

		static Attachment LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

}