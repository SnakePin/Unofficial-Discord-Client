#include <iostream>

#include "discord/discord.hpp"


class MyListener : public Discord::Listener {
public:

	void on_message_receive(Discord::Message message) {
		std::cout << "Received message: " << message.getContent() << std::endl;
	}
	
};

int main(int argc, char **argv) {
	// $ ./Unofficial-Discord-Client [discord token]
	Discord::Client client( (argc == 2)? argv[1] : "token");
	MyListener listener;
	
	client.addListener(&listener);
	
	client.run();
	
	// std::cout << client.generate_identify_packet() << std::endl;
	
}
