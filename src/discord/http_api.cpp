#include <discord/discord.hpp>

using namespace Discord;

Client::HTTP_API::HTTP_API(const Client& clientObj)
    : token(clientObj.token) {
	
}

Client::HTTP_API::HTTP_API(const AuthToken _token)
    : token(_token) {
	
}

//Client::HTTP_API::SendMessage()
