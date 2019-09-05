#pragma once

#include "discord/snowflake.hpp"

#include <rapidjson/document.h>

#include <optional>

namespace Discord {

	struct User {
		// https://discordapp.com/developers/docs/resources/user#user-object
		
		Snowflake id;
		std::string username;
		std::string discriminator;
		std::string avatarHash;
		
		std::optional<bool> bot;
		std::optional<bool> mfaEnabled;
		std::optional<std::string> locale;
		std::optional<uint32_t> flags;
		std::optional<int32_t> premiumType;

		static User LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}