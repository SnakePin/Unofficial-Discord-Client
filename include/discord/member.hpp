#pragma once

#include "discord/snowflake.hpp"
#include "discord/user.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

#include <rapidjson/document.h>

#include <vector>
#include <optional>

namespace Discord {

	struct Member : Deserializable_Serializable_Class<Member> {
		// https://discordapp.com/developers/docs/resources/guild#guild-member-object
		
		User user;
		std::optional<std::string>  nickname;
		std::vector<Snowflake> roles;
		std::string  joinedAt;
		std::string  premiumSince;

		bool deaf;
		bool mute;

		static Member LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

}