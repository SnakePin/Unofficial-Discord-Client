#pragma once

#include "discord/snowflake.hpp"
#include "discord/user.hpp"
#include "discord/jsonDeSerializable_class.hpp"

#include <rapidjson/document.h>

#include <vector>
#include <optional>

namespace Discord {

	struct Member : JsonDeSerializable {
		// https://discordapp.com/developers/docs/resources/guild#guild-member-object
		
		User user;
		std::optional<std::string>  nickname;
		std::vector<Snowflake> roles;
		std::string  joinedAt;
		std::string  premiumSince;

		bool deaf;
		bool mute;

		bool LoadFrom(rapidjson::Value& object);
	};

}