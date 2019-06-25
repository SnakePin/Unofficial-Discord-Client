#pragma once

#include <string>
#include <sstream>
#include <cstdint>

namespace Discord {

	struct Snowflake {
		uint64_t value;

		Snowflake() : value(0) {}

		void operator=(const Snowflake &other) {
			value = other.value;
		}

		Snowflake(std::string s) : value(0) {
			std::istringstream iss(s);
			iss >> value;
		}

		Snowflake(uint64_t v) : value(v) {}

		// Unix epoch in milliseconds
		inline uint64_t UnixEpoch() {
			return (value >> 22) + 1420070400000;
		}

		inline bool operator==(const Snowflake &other) {
			return other.value == value;
		}

		inline bool operator!=(const Snowflake &other) {
			return other.value != value;
		}
	};

}
