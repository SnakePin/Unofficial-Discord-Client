#include <discord/token.hpp>

using namespace Discord;

AuthToken::AuthToken(std::string token, AuthTokenType tokenType)
    : token(token), tokenType(tokenType) {
	
}