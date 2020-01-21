#include "discord/InternalUtils.hpp"
#include "discord/token.hpp"
#include <string>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <cpprest/http_client.h>

using utility::conversions::to_string_t;
using utility::conversions::to_utf8string;

namespace Discord::InternalUtils
{
	std::string JsonDocumentToJsonString(rapidjson::Document& jsonDocument)
	{
		rapidjson::StringBuffer _stringBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(_stringBuffer);
		jsonDocument.Accept(writer);
		return _stringBuffer.GetString();
	}

	std::string AuthTokenToAuthHeaderValue(const AuthToken& token)
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

	//Warning: This function can only be called once on the same http_response instance
	std::string cpprestsdkResponse_to_Utf8StdString(const web::http::http_response& response)
	{
		return to_utf8string(response.extract_string(true).get());
	}

	void SimpleAsyncTimedEvent::cancel() {
		cv.notify_one();
	}
	void SimpleAsyncTimedEvent::doLater(int afterMS, std::function<void(void)> callback)
	{
		if (_isRunning) {
			return;
		}
		_isRunning = true;

		internalFuture = std::async([&, afterMS, callback]() {
			std::mutex mtx;
			std::unique_lock<std::mutex> lck(mtx);
			cv.wait_for(lck, std::chrono::milliseconds(afterMS));

			callback();

			_isRunning = false;
		});
	}
	void SimpleAsyncTimedEvent::wait() {
		internalFuture.wait();
	}
	bool SimpleAsyncTimedEvent::isRunning() {
		return _isRunning;
	}
}