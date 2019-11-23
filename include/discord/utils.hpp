#pragma once

#include <string_view>
#include <rapidjson/document.h>

#include "discord/utils.hpp"
#include <string_view>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>

namespace Discord::Utils
{
	std::string_view JsonDocumentToJsonString(rapidjson::Document& jsonDocument);
}
