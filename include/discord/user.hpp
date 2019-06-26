#pragma once

#include <discord/snowflake.hpp>

#include <rapidjson/document.h>

namespace Discord {

	struct User {
		// https://discordapp.com/developers/docs/resources/user#user-object
		
		Snowflake id;
		std::optional<std::string> username;
		std::optional<std::string> discriminator;
		std::optional<std::string> avatarHash;
		
		std::optional<bool> bot;
		std::optional<bool> mfaEnabled;
		std::optional<std::string> locale;
		std::optional<uint32_t> flags;
		std::optional<int32_t> premiumType;

		static User LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}