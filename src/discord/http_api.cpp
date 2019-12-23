#include "discord/discord.hpp"
#include "discord/utils.hpp"

#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>

#include <string>
#include <cpr/cpr.h>
#include <tinyformat.h>

using namespace Discord;
using namespace Utils;

static std::string AuthTokenToAuthHeaderValue(const AuthToken& token);

//TODO: Fix the ssl issue as soon as possible
//TODO: Handle API errors and return a bool according to succession status, maybe even return error objects.

Client::HTTP_API_CLASS::HTTP_API_CLASS(const Client& clientObj)
	: HTTP_API_CLASS(clientObj.token)
{

}

Client::HTTP_API_CLASS::HTTP_API_CLASS(const AuthToken _token)
	: token(_token),
	userAgent(DefaultUserAgentString)
{
	
}

//Do not change the return value's type to string_view, CPR doesn't accept it
static std::string AuthTokenToAuthHeaderValue(const AuthToken& token)
{
	std::string headerValue;

	if (token.tokenType == AuthTokenType::BOT) {
		headerValue = "Bot " + token.value;
	}
	else {
		headerValue = token.value;
	}

	return headerValue;
}

bool Client::HTTP_API_CLASS::CreateMessage(const Discord::Snowflake &channelID, Discord::CreateMessageParam messageToSend)
{
	rapidjson::Document JsonDocument;
	rapidjson::Pointer("/content").Set(JsonDocument, messageToSend.content.c_str());
	rapidjson::Pointer("/tts").Set(JsonDocument, messageToSend.tts);

	std::string postBody = JsonDocumentToJsonString(JsonDocument);

	cpr::Response response = cpr::Post(cpr::Url{tfm::format("https://discordapp.com/api/v6/channels/%s/messages", channelID.value)},
										cpr::Body{postBody},
										cpr::Header
										{
											{"Authorization", AuthTokenToAuthHeaderValue(token)},
											{"User-Agent", userAgent},
											{"Content-Type", "application/json"}
										},
										cpr::VerifySsl{false});
	return response.status_code == 200;
}

bool Client::HTTP_API_CLASS::DeleteMessage(const Snowflake &channelID, const Snowflake &messageID) 
{
	cpr::Response response = cpr::Delete(cpr::Url{tfm::format("https://discordapp.com/api/v6/channels/%s/messages/%s", channelID.value, messageID.value)},
										cpr::Header
										{
											{"Authorization", AuthTokenToAuthHeaderValue(token)},
											{"User-Agent", userAgent}
										},
										cpr::VerifySsl{false});
	return response.status_code == 204;
}

bool Client::HTTP_API_CLASS::TriggerTypingIndicator(const Discord::Snowflake &channelID)
{
	cpr::Response response = cpr::Post(cpr::Url{tfm::format("https://discordapp.com/api/v6/channels/%s/typing", channelID.value)},
										cpr::Header
										{
											{"Authorization", AuthTokenToAuthHeaderValue(token)},
											{"Content-Length", "0"},
											{"User-Agent", userAgent}
										},
										cpr::VerifySsl{false});
	return response.status_code == 204;
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

		for(rapidjson::SizeType i=0; i<document.GetArray().Size(); i++) {
			Message tmpVar;
			tmpVar.LoadFrom(document, tfm::format("/%d", i));
			messages.push_back(
				tmpVar
			);
		}

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::GetChannel(const Snowflake &channelID, Channel& channel) {
	cpr::Response response = cpr::Get(
		cpr::Url{tfm::format("https://discordapp.com/api/v6/channels/%s", channelID.value)},
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

		channel.LoadFrom(document);

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::GetChannelMessage(const Snowflake &channelID, const Snowflake &messageID, Message &message) {
	cpr::Response response = cpr::Get(
		cpr::Url{tfm::format("https://discordapp.com/api/v6/channels/%s/messages/%s", channelID.value, messageID.value)},
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

		message.LoadFrom(document);

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::GetPinnedMessages(const Snowflake &channelID, std::vector<Message>& messages) {
	cpr::Response response = cpr::Get(
		cpr::Url{tfm::format("https://discordapp.com/api/v6/channels/%s/pins", channelID.value)},
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

		for(rapidjson::SizeType i=0; i<document.GetArray().Size(); i++) {
			Message tmpVar;
			tmpVar.LoadFrom(document, tfm::format("/%d", i));
			messages.push_back(
				tmpVar
			);
		}

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::UnpinChannelMessage(const Snowflake &channelID, const Snowflake &messageID) {
	cpr::Response response = cpr::Delete(cpr::Url{tfm::format("https://discordapp.com/api/v6/channels/%s/pins/%s", channelID.value, messageID.value)},
										cpr::Header
										{
											{"Authorization", AuthTokenToAuthHeaderValue(token)},
											{"User-Agent", userAgent}
										},
										cpr::VerifySsl{false});
	return response.status_code == 204;
}

bool Client::HTTP_API_CLASS::GetCurrentUser(User &user) {
	cpr::Response response = cpr::Get(
		cpr::Url{"https://discordapp.com/api/v6/users/@me"},
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

		user.LoadFrom(document);

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::GetUserByID(User &user, const Snowflake &userID) {
	cpr::Response response = cpr::Get(
		cpr::Url{tfm::format("https://discordapp.com/api/v6/users/%s", userID.value)},
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

		user.LoadFrom(document);

		return true;
	}

	return false;
}
