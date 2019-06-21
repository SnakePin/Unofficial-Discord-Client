#pragma once

#include <discord/snowflake.hpp>

#include <rapidjson/document.h>

namespace Discord {

	struct User {
		// https://discordapp.com/developers/docs/resources/user#user-object
		
		Snowflake id;
		std::string username;
		std::string discriminator;
		std::string avatarHash;
		
		bool bot;
		bool mfaEnabled;
		std::string locale;
		uint32_t flags;
		int32_t premiumType;

		static User LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}