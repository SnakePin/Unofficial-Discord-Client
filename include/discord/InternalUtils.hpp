#pragma once

#include <string>
#include <future>
#include <atomic>
#include <functional>
#include <type_traits>

#include <rapidjson/document.h>
#include "discord/token.hpp"
#include <cpprest/http_client.h>
#include <condition_variable>

namespace Discord::InternalUtils
{
	std::string JsonDocumentToJsonString(rapidjson::Document& jsonDocument);
	std::string AuthTokenToAuthHeaderValue(const AuthToken& token);

	template<typename Function>
	inline void CallFunctionAsynchronouslyAndForget(Function func) {
		std::thread(func).detach();
	}

	//Warning: This function can only be called once on the same http_response instance
	std::string cpprestsdkResponse_to_Utf8StdString(const web::http::http_response& response);

	class SimpleAsyncTimedEvent
	{
		std::atomic<bool> _isRunning = false;
		std::future<void> internalFuture;
		std::condition_variable cv;
	public:
		void cancel();
		void doLater(int afterMS, std::function<void(void)> callback);
		void wait();
		bool isRunning();
	};

}
