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

		Snowflake(std::string s) : Snowflake() {
			std::istringstream iss(s);
			iss >> value;
		}

		Snowflake(uint64_t v) : value(v) {}

		// Unix epoch in milliseconds
		uint64_t GetUnixEpoch() const {
			return (value >> 22) + 1420070400000;
		}
		uint8_t GetInternalWorkerID() const {
			return (uint8_t)((value & 0x3E0000) >> 17);
		}
		uint8_t GetInternalProcessID() const {
			return (uint8_t)((value & 0x1F000) >> 12);
		}
		uint16_t GetIncrement() const {
			return value & 0xFFF;
		}

		inline bool operator==(const Snowflake &rhs) const {
			return rhs.value == value;
		}

		inline bool operator!=(const Snowflake &rhs) const {
			return rhs.value != value;
		}
		inline operator uint64_t() const {
			return value;
		} 
	};

}
