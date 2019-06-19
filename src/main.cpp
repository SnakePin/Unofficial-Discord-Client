#include <iostream>

#include "discord/discord.hpp"

class MyListener : public Discord::Listener {
public:

	void on_message_receive(Discord::Message message) {
		std::cout << "Received message: " << message.getContent() << std::endl;
	}
	
};

int main() {
	Discord::Client client("token");
	MyListener listener;
	
	client.addListener(&listener);
	
	client.run();
}
