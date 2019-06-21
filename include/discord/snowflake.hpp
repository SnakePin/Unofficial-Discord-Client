#pragma once

#include <string>
#include <sstream>
#include <cstdint>

namespace Discord {

	struct Snowflake {
		std::string original;
		uint64_t value;

		Snowflake() {

		}

		void operator=(const Snowflake &other) {
			original = other.original;
			value = other.value;
		}

		Snowflake(std::string s) : original(s), value(0) {
			std::istringstream iss(original);
			iss >> value;
		}

		Snowflake(uint64_t v) : original(std::to_string(v)), value(v) {}

		inline uint64_t UnixEpoch() {
			return (value >> 22) + 1420070400000;
		}
	};

	class ISnowflake {
		public:
			Snowflake id;
	};
}
