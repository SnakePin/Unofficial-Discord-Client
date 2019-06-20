#pragma once
#include <string>

namespace Discord {
    enum class AuthTokenType {
        USER,
        BOT,
        BEARER
    };

	struct AuthToken {
        AuthToken(std::string _token, AuthTokenType _tokenType);

		std::string token;
		AuthTokenType tokenType;
	};
}