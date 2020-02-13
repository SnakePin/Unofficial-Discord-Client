#include "discord/discord.hpp"
#include "discord/InternalUtils.hpp"

#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>

#include <string>
#include <cpprest/http_client.h>
#include <tinyformat.h>

using namespace Discord;
using namespace InternalUtils;
using utility::conversions::to_string_t;
using utility::conversions::to_utf8string;

//TODO: Handle API errors and return a bool according to succession status, maybe even return error objects.

Client::HTTP_API_CLASS::HTTP_API_CLASS(const Client& clientObj)
	: HTTP_API_CLASS(clientObj.authToken)
{

}

Client::HTTP_API_CLASS::HTTP_API_CLASS(const AuthToken authToken_arg)
	: authToken(authToken_arg),
	userAgent(DefaultUserAgentString)
{
	
}

bool Client::HTTP_API_CLASS::CreateMessage(const Discord::Snowflake &channelID, Discord::CreateMessageParam messageToSend)
{
	rapidjson::Document JsonDocument;
	rapidjson::Pointer("/content").Set(JsonDocument, messageToSend.content);
	rapidjson::Pointer("/tts").Set(JsonDocument, messageToSend.tts);
	
    web::uri_builder builder(U("channels"));
	builder.append_path(to_string_t(std::to_string(channelID.value)));
	builder.append_path(U("messages"));

	web::http::http_request request(web::http::methods::POST);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.set_body(to_string_t(JsonDocumentToJsonString(JsonDocument)), U("application/json"));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	return response.status_code() == 200;
}

bool Client::HTTP_API_CLASS::DeleteMessage(const Snowflake &channelID, const Snowflake &messageID) 
{
	web::uri_builder builder(U("channels"));
	builder.append_path(to_string_t(std::to_string(channelID.value)));
	builder.append_path(U("messages"));
	builder.append_path(to_string_t(std::to_string(messageID.value)));

	web::http::http_request request(web::http::methods::DEL);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	return response.status_code() == 204;
}

bool Client::HTTP_API_CLASS::TriggerTypingIndicator(const Discord::Snowflake &channelID)
{
	web::uri_builder builder(U("channels"));
	builder.append_path(to_string_t(std::to_string(channelID.value)));
	builder.append_path(U("typing"));

	web::http::http_request request(web::http::methods::POST);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.headers().add(U("Content-Length"), U("0"));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	return response.status_code() == 204;
}

bool Client::HTTP_API_CLASS::GetMessagesInto(const Snowflake &channelID, std::vector<Message>& messages, int count)
{
	web::uri_builder builder(U("channels"));
	builder.append_path(to_string_t(std::to_string(channelID.value)));
	builder.append_path(U("messages"));
	builder.append_query(to_string_t(tfm::format("limit=%d", count)));

	web::http::http_request request(web::http::methods::GET);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	if(response.status_code() == 200) {
		rapidjson::Document document;
		document.Parse(cpprestsdkResponse_to_Utf8StdString(response));

		if(!document.IsArray()) return false;

		for(rapidjson::Value& message : document.GetArray()) {
			Message tmpVar;
			tmpVar.LoadFrom(message);
			messages.push_back(tmpVar);
		}

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::GetChannel(const Snowflake &channelID, Channel& channel)
{
	web::uri_builder builder(U("channels"));
	builder.append_path(to_string_t(std::to_string(channelID.value)));

	web::http::http_request request(web::http::methods::GET);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	if(response.status_code() == 200) {
		rapidjson::Document document;
		document.Parse(cpprestsdkResponse_to_Utf8StdString(response));
		channel.LoadFrom(document);

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::GetChannelMessage(const Snowflake &channelID, const Snowflake &messageID, Message &message)
{
	web::uri_builder builder(U("channels"));
	builder.append_path(to_string_t(std::to_string(channelID.value)));
	builder.append_path(U("messages"));
	builder.append_path(to_string_t(std::to_string(messageID.value)));

	web::http::http_request request(web::http::methods::GET);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	if(response.status_code() == 200) {
		rapidjson::Document document;
		document.Parse(cpprestsdkResponse_to_Utf8StdString(response));
		message.LoadFrom(document);

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::GetPinnedMessages(const Snowflake &channelID, std::vector<Message>& messages)
{
	web::uri_builder builder(U("channels"));
	builder.append_path(to_string_t(std::to_string(channelID.value)));
	builder.append_path(U("pins"));

	web::http::http_request request(web::http::methods::GET);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	if(response.status_code() == 200) {
		rapidjson::Document document;
		document.Parse(cpprestsdkResponse_to_Utf8StdString(response));

		if(!document.IsArray()) return false;

		for(rapidjson::Value& message : document.GetArray()) {
			Message tmpVar;
			tmpVar.LoadFrom(message);
			messages.push_back(tmpVar);
		}

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::UnpinChannelMessage(const Snowflake &channelID, const Snowflake &messageID)
{
	web::uri_builder builder(U("channels"));
	builder.append_path(to_string_t(std::to_string(channelID.value)));
	builder.append_path(U("pins"));
	builder.append_path(to_string_t(std::to_string(messageID.value)));

	web::http::http_request request(web::http::methods::DEL);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	return response.status_code() == 204;
}

bool Client::HTTP_API_CLASS::GetCurrentUser(User &user)
{
	web::uri_builder builder(U("users"));
	builder.append_path(U("@me"));

	web::http::http_request request(web::http::methods::GET);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	if(response.status_code() == 200) {
		rapidjson::Document document;
		document.Parse(cpprestsdkResponse_to_Utf8StdString(response));
		user.LoadFrom(document);

		return true;
	}

	return false;
}

bool Client::HTTP_API_CLASS::GetUserByID(User &user, const Snowflake &userID)
{
	web::uri_builder builder(U("users"));
	builder.append_path(to_string_t(std::to_string(userID.value)));

	web::http::http_request request(web::http::methods::GET);
	request.headers().add(U("Authorization"), to_string_t(AuthTokenToAuthHeaderValue(authToken)));
	request.headers().add(U("User-Agent"), to_string_t(userAgent));
	request.set_request_uri(builder.to_uri());

	auto response = client.request(request).get();

	if(response.status_code() == 200) {
		rapidjson::Document document;
		document.Parse(cpprestsdkResponse_to_Utf8StdString(response));
		user.LoadFrom(document);

		return true;
	}

	return false;
}
