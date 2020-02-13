#pragma once
#include <rapidjson/document.h>
#include <string>
#include <stdint.h>
#include <optional>
#include <type_traits>
#include <vector>
#include "discord/snowflake.hpp"
#include "discord/jsonDeSerializable_class.hpp"

//Begin declarations
bool JsonTypeToStructType(rapidjson::Value& object, Discord::JsonDeSerializable &structFieldRef);
bool JsonTypeToStructType(rapidjson::Value& object, Discord::Snowflake &structFieldRef);
bool JsonTypeToStructType(rapidjson::Value& object, std::string &structFieldRef);
template <typename T>
bool JsonTypeToStructType(rapidjson::Value& object, std::optional<T> &structFieldRef);
template <typename T>
bool JsonTypeToStructType(rapidjson::Value& object, std::vector<T> &structFieldRef);
template <typename T, typename std::enable_if<
			std::is_same<bool, T>::value || 
			std::is_same<int, T>::value ||
			std::is_same<unsigned int, T>::value ||
			std::is_same<int64_t, T>::value ||
			std::is_same<uint64_t, T>::value ||
			std::is_same<double, T>::value ||
			std::is_same<float, T>::value ||
			std::is_same<uint64_t, T>::value>::type* = nullptr>
bool JsonTypeToStructType(rapidjson::Value& object, T &structFieldRef);
template <typename T, typename std::enable_if<
			std::is_same<uint16_t, T>::value || 
			std::is_same<uint8_t, T>::value>::type* = nullptr>
bool JsonTypeToStructType(rapidjson::Value& object, T &structFieldRef);
template <typename T, typename std::enable_if<
			std::is_same<int16_t, T>::value || 
			std::is_same<int8_t, T>::value>::type* = nullptr>
bool JsonTypeToStructType(rapidjson::Value& object, T &structFieldRef);
//End declarations

template <typename T, typename std::enable_if<
			std::is_same<uint16_t, T>::value || 
			std::is_same<uint8_t, T>::value>::type*>
bool JsonTypeToStructType(rapidjson::Value& object, T &structFieldRef)
{
	if(object.IsUint()) {
		structFieldRef = (T)object.GetUint();
		return true;
	}
	return false;
}

template <typename T, typename std::enable_if<
			std::is_same<int16_t, T>::value || 
			std::is_same<int8_t, T>::value>::type*>
bool JsonTypeToStructType(rapidjson::Value& object, T &structFieldRef)
{
	if(object.IsInt()) {
		structFieldRef = (T)object.GetInt();
		return true;
	}
	return false;
}
template <typename T>
bool JsonTypeToStructType(rapidjson::Value& object, std::optional<T> &structFieldRef)
{
	T tempVar;
	if (JsonTypeToStructType(object, tempVar))
	{
		structFieldRef = tempVar;
		return true;
	}
	return false;
}

template <typename T>
bool JsonTypeToStructType(rapidjson::Value& object, std::vector<T> &structFieldRef)
{
	if (object.IsArray())
	{
		for (rapidjson::Value& currentObject : object.GetArray()) {
			T tempVar;
			JsonTypeToStructType(currentObject, tempVar);
			structFieldRef.push_back(tempVar);
		}
		return true;
	}
	return false;
}

template <typename T, typename std::enable_if<
			std::is_same<bool, T>::value || 
			std::is_same<int, T>::value ||
			std::is_same<unsigned int, T>::value ||
			std::is_same<int64_t, T>::value ||
			std::is_same<uint64_t, T>::value ||
			std::is_same<double, T>::value ||
			std::is_same<float, T>::value ||
			std::is_same<uint64_t, T>::value>::type*>
bool JsonTypeToStructType(rapidjson::Value& object, T &structFieldRef)
{
	if (object.Is<T>())
	{
		structFieldRef = object.Get<T>();
		return true;
	}
	return false;
}
