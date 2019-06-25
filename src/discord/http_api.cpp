#include <discord/discord.hpp>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <cpr/cpr.h>

using namespace Discord;

static std::string AuthTokenToAuthHeaderValue(AuthToken token);
static std::string JsonDocumentToJsonString(rapidjson::Document& jsonDocument);

Client::HTTP_API_CLASS::HTTP_API_CLASS(const Client& clientObj)
    : token(clientObj.token) {

}

Client::HTTP_API_CLASS::HTTP_API_CLASS(const AuthToken _token)
    : token(_token) {
	
}

static std::string AuthTokenToAuthHeaderValue(AuthToken token)
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

void Client::HTTP_API_CLASS::SendMessage(std::string channelID, Discord::MessagePacket messageToSend)
{
    rapidjson::Document JsonDocument;
    rapidjson::Pointer("/content").Set(JsonDocument, messageToSend.content.c_str());
    rapidjson::Pointer("/tts").Set(JsonDocument, messageToSend.tts);

    std::string postBody = JsonDocumentToJsonString(JsonDocument);

    cpr::Response response = cpr::Post(cpr::Url{"https://discordapp.com/api/v6/channels/"+channelID+"/messages"},
                                       cpr::Body{postBody},
                                       cpr::Header{{"Content-Type", "application/json"}},
                                       cpr::Header{{"Authorization", AuthTokenToAuthHeaderValue(token)}},
                                       cpr::VerifySsl{false});
    //response.status_code;                  // 200
    //response.header["content-type"];       // application/json; charset=utf-8
    //response.text;  
}

void Client::HTTP_API_CLASS::StartTyping(std::string channelID)
{
    cpr::Response response = cpr::Post(cpr::Url{"https://discordapp.com/api/v6/channels/"+channelID+"/typing"},
                                       cpr::Header{{"Authorization", AuthTokenToAuthHeaderValue(token)}},
                                       cpr::VerifySsl{false});
}
