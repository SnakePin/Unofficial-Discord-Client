#pragma once

#include <string>
#include <rapidjson/document.h>

namespace Discord {
	class JsonDeSerializable {
	public:
		//If deconstructor is not virtual, it would lead to memory leak if an base class instance that is actually a derived instance is deleted
		virtual ~JsonDeSerializable() = default;

		virtual bool LoadFrom(rapidjson::Document& doc, std::string pointer = "") = 0;

		//TODO implement this: bool SerializeTo(rapidjson::Document& doc, std::string pointer = "")
	};
}