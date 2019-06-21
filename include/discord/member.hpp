#pragma once

#include <discord/snowflake.hpp>
#include <discord/user.hpp>

#include <rapidjson/document.h>

#include <vector>

namespace Discord {

	struct Member {
		// https://discordapp.com/developers/docs/resources/guild#guild-member-object
		
		User user;
		std::string nickname;
		std::vector<Snowflake> roles;
		std::string joinedAt;
		std::string premiumSince;

		bool deaf;
		bool mute;

		static Member LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}