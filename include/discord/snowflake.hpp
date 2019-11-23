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
		inline uint64_t GetUnixEpoch() {
			return (value >> 22) + 1420070400000;
		}
		inline uint8_t GetInternalWorkerID() {
			return (value & 0x3E0000) >> 17;
		}
		inline uint8_t GetInternalProcessID() {
			return (value & 0x1F000) >> 12;
		}
		inline uint16_t GetIncrement() {
			return value & 0xFFF;
		}

		inline bool operator==(const Snowflake &other) {
			return other.value == value;
		}

		inline bool operator!=(const Snowflake &other) {
			return other.value != value;
		}
	};

}
