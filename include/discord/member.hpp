#pragma once

#include <discord/snowflake.hpp>
#include <discord/user.hpp>

#include <rapidjson/document.h>

#include <vector>
#include <optional>

namespace Discord {

	struct Member {
		// https://discordapp.com/developers/docs/resources/guild#guild-member-object
		
		User user;
		std::optional<std::string>  nickname;
		std::vector<Snowflake> roles;
		std::optional<std::string>  joinedAt;
		std::optional<std::string>  premiumSince;

		std::optional<bool> deaf;
		std::optional<bool> mute;

		static Member LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}