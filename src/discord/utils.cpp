#include "discord/utils.hpp"
#include <string>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>

namespace Discord::Utils
{
	std::string JsonDocumentToJsonString(rapidjson::Document &jsonDocument)
	{
		rapidjson::StringBuffer _stringBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(_stringBuffer);
		jsonDocument.Accept(writer);
		return _stringBuffer.GetString();
	}
}