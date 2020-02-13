#pragma once

#include "discord/snowflake.hpp"
#include "discord/jsonDeSerializable_class.hpp"

#include <rapidjson/document.h>
#include <stdint.h>

namespace Discord {

	struct UserFlags : JsonDeSerializable {
		// https://discordapp.com/developers/docs/resources/user#user-object-user-flags

		int16_t flags;

		//TODO: find out why the following functions aren't working
		inline bool IsNone() {
			return (flags == 0);
		}
		inline bool IsDiscordEmployee() {
			return (flags << 0) & 1;
		}
		inline bool IsDiscordPartner() {
			return (flags << 1) & 1;
		}
		inline bool IsHypeSquadEvents() {
			return (flags << 2) & 1;
		}
		inline bool IsBugHunter() {
			return (flags << 3) & 1;
		}
		inline bool IsHouseBravery() {
			return (flags << 6) & 1;
		}
		inline bool IsHouseBrilliance() {
			return (flags << 7) & 1;
		}
		inline bool IsHouseBalance() {
			return (flags << 8) & 1;
		}
		inline bool IsEarlySupporter() {
			return (flags << 9) & 1;
		}
		inline bool IsTeamUser() {
			return (flags << 10) & 1;
		}
		inline bool IsSystem() {
			return (flags << 12) & 1;
		}

		bool LoadFrom(rapidjson::Value& object);
	};

}