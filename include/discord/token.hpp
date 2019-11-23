#pragma once
#include <string>

namespace Discord {
    enum class AuthTokenType {
        USER,
        BOT
    };

	struct AuthToken {
        AuthToken(std::string _value, AuthTokenType _tokenType) : value(_value), tokenType(_tokenType) { };

		std::string value;
		AuthTokenType tokenType;
	};
}