#pragma once

#include <string>
#include <rapidjson/document.h>

namespace Discord {

	//This class uses the CRTP pattern to hold type information which will be later used in de-serialize function
	//Warning: If you are making a class that derives from this class, you MUST have functions with the following signatures
	//static T LoadFrom(rapidjson::Document&, std::string)
	//static T SerializeTo(rapidjson::Document&, std::string) (Not required yet, TODO implement this)
	template <typename T>
	class Deserializable_Serializable_Class {
	public:
		//If deconstructor is not virtual, it would lead to memory leak if an base class instance that is actually an derived instance is deleted
		virtual ~Deserializable_Serializable_Class() = default;
	};
}