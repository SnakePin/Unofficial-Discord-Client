#include <discord/discord.hpp>

using namespace Discord;

Client::HTTP_API::HTTP_API(const Client& clientObj)
    : tokenRef(clientObj.token) {
	
}
