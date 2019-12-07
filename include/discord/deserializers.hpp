#pragma once
#include <rapidjson/document.h>
#include <string>
#include <stdint.h>
#include <optional>
#include <vector>
#include "discord/snowflake.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, std::string& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint32_t& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int32_t& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int64_t& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint64_t& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint8_t& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int8_t& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint16_t& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int16_t& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, bool& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, float& structFieldRef);
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, double& structFieldRef);

//Warning: This function has no error checking, it will return true even if json field was not found
//This function will get called if structFieldRef is a reference to a class that implements Discord::Deserializable_Serializable_Class<T>.
//The template will be deduced from the derived class.
template <typename T>
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, Discord::Deserializable_Serializable_Class<T>& structFieldRef);

bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, Discord::Snowflake& structFieldRef);
template <typename T>
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, std::optional<T>& structFieldRef);
template <typename T>
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, std::vector<T>& structFieldRef);