#pragma once

#include "discord/snowflake.hpp"
#include "discord/userFlags.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

#include <rapidjson/document.h>
#include <optional>

namespace Discord {

	struct User : JsonDeSerializable {
		// https://discordapp.com/developers/docs/resources/user#user-object
		
		Snowflake id;
		std::string username;
		std::string discriminator;
		std::string avatarHash;
		
		std::optional<bool> bot;
		std::optional<bool> mfaEnabled;
		std::optional<std::string> locale;
		std::optional<bool> verified;
		std::optional<std::string> email;
		std::optional<UserFlags> flags;
		std::optional<int32_t> premiumType;

		bool LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

}