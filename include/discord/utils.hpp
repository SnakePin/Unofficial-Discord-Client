#pragma once

#include <string>
#include <rapidjson/document.h>

namespace Discord::Utils
{
	std::string JsonDocumentToJsonString(rapidjson::Document& jsonDocument);
}
