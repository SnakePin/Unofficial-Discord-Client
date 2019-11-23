#include "discord/utils.hpp"
#include <string_view>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>

namespace Discord::Utils
{
	std::string_view JsonDocumentToJsonString(rapidjson::Document &jsonDocument)
	{
		rapidjson::StringBuffer _stringBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(_stringBuffer);
		jsonDocument.Accept(writer);
		return std::string_view(_stringBuffer.GetString());
	}
}