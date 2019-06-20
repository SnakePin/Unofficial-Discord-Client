#include <discord/discord.hpp>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <cpr/cpr.h>

using namespace Discord;

Client::HTTP_API_CLASS::HTTP_API_CLASS(const Client& clientObj)
    : token(clientObj.token) {
	
}

Client::HTTP_API_CLASS::HTTP_API_CLASS(const AuthToken _token)
    : token(_token) {
	
}

void Client::HTTP_API_CLASS::SendMessage(std::string channelID, Discord::Message messageToSend)
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

    rapidjson::Document JsonDocument;
    rapidjson::Pointer("/content").Set(JsonDocument, messageToSend.content.c_str());
    rapidjson::Pointer("/tts").Set(JsonDocument, false);

    rapidjson::StringBuffer _stringBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(_stringBuffer);
	JsonDocument.Accept(writer);

    cpr::Response response = cpr::Post(cpr::Url{"https://discordapp.com/api/v6/channels/"+channelID+"/messages"},
                                       cpr::Body{_stringBuffer.GetString()},
                                       cpr::Header{{"Content-Type", "application/json"}},
                                       cpr::Header{{"Authorization", authHeaderValue}},
                                       cpr::VerifySsl{false});
    //response.status_code;                  // 200
    //response.header["content-type"];       // application/json; charset=utf-8
    //response.text;  
}
