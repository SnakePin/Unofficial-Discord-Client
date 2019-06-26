#pragma once

#include <discord/user.hpp>
#include <discord/guild.hpp>

#include <vector>
#include <optional>

namespace Discord {

	struct ReadyPacket {
		int32_t version;
		User user;

		std::vector<Guild> guilds;
		std::string sessionID;

		std::optional<std::vector<int32_t>> shard;

		static ReadyPacket LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

	struct TypingStartPacket {
		Snowflake userID;
		std::optional<Snowflake> guildID;
		Snowflake channelID;

		std::optional<Member> member;

		uint64_t timestamp;
		
		static TypingStartPacket LoadFrom(rapidjson::Document &doc, std::string pointer);
	};

}