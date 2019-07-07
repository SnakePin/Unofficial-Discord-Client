#include <discord/discord.hpp>

#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <cpr/cpr.h>

#include <tinyformat.h>

using namespace Discord;

static std::string AuthTokenToAuthHeaderValue(const AuthToken& token);
static std::string JsonDocumentToJsonString(rapidjson::Document& jsonDocument);

Client::HTTP_API_CLASS::HTTP_API_CLASS(const Client& clientObj)
    : HTTP_API_CLASS(clientObj.token) {

}

Client::HTTP_API_CLASS::HTTP_API_CLASS(const AuthToken _token)
    : token(_token),
	userAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/75.0.3770.90 Chrome/75.0.3770.90 Safari/537.36") {
	
}

static std::string AuthTokenToAuthHeaderValue(const AuthToken& token)
{
    std::string authHeaderValue;

    if (token.tokenType == AuthTokenType::BOT) {
        authHeaderValue = "Bot " + token.token;
    }
    else if(token.tokenType == AuthTokenType::BEARER){
        authHeaderValue = "Bearer " + token.token;
    }
    else {
        authHeaderValue = token.token;
    }

    return authHeaderValue;
}

static std::string JsonDocumentToJsonString(rapidjson::Document& jsonDocument)
{
    rapidjson::StringBuffer _stringBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(_stringBuffer);
	jsonDocument.Accept(writer);
    return std::string(_stringBuffer.GetString());
}

void Client::HTTP_API_CLASS::SendMessage(const Discord::Snowflake &channelID, Discord::MessagePacket messageToSend)
{
    rapidjson::Document JsonDocument;
    rapidjson::Pointer("/content").Set(JsonDocument, messageToSend.content.c_str());
    rapidjson::Pointer("/tts").Set(JsonDocument, messageToSend.tts);

    std::string postBody = JsonDocumentToJsonString(JsonDocument);

    cpr::Response response = cpr::Post(cpr::Url{"https://discordapp.com/api/v6/channels/"+std::to_string(channelID.value)+"/messages"},
                                       cpr::Body{postBody},
                                       cpr::Header
										{
										   {"Authorization", AuthTokenToAuthHeaderValue(token)},
										   {"Content-Type", "application/json"},
										   {"User-Agent", userAgent}
										},
                                       cpr::VerifySsl{false});
    //response.status_code;                  // 200
    //response.header["content-type"];       // application/json; charset=utf-8
    //response.text;  
}

void Client::HTTP_API_CLASS::StartTyping(const Discord::Snowflake &channelID)
{
    cpr::Response response = cpr::Post(cpr::Url{"https://discordapp.com/api/v6/channels/"+std::to_string(channelID.value)+"/typing"},
                                       cpr::Header
                                       {
                                            {"Authorization", AuthTokenToAuthHeaderValue(token)},
                                            {"Content-Length", "0"},
											{"User-Agent", userAgent}
                                       },
                                       cpr::VerifySsl{false});
}

bool Client::HTTP_API_CLASS::GetMessagesInto(const Snowflake &channelID, std::vector<Message>& messages, int count) {
	cpr::Response response = cpr::Get(
		cpr::Url{tfm::format("https://discordapp.com/api/v6/channels/%s/messages?limit=%d", channelID.value, count)},
		cpr::Header
		{
			{"Authorization", AuthTokenToAuthHeaderValue(token)},
			{"User-Agent", userAgent}
		},
		cpr::VerifySsl{false}
	);

	if(response.status_code == 200) {
		rapidjson::Document document;
		document.Parse(response.text.c_str());

		if(!document.IsArray()) return false;

		for(int i=0; i<document.GetArray().Size(); i++) {
			messages.push_back(
				Message::LoadFrom(document, tfm::format("/%d", i))
			);
		}

		return true;
	}

	return false;
}