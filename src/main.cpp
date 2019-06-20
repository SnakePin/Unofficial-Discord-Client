#include <iostream>
#include <thread>

#include "discord/discord.hpp"


class MyListener : public Discord::Listener {
public:

	void on_message_receive(Discord::Message message) {
		std::cout << "Received message: " << message.getContent() << std::endl;
	}
	
};

class ConsoleTest {
private:

	std::shared_ptr<Discord::Client> client;
	bool running;

public:
	ConsoleTest(std::shared_ptr<Discord::Client> client)
	    : client(client), running(false) {

	}

	void processCommand(std::string command) {
		if(command == "quit") {
			std::cout << "Stopping the websocket...\n";
			client->websocket.stop();
			std::cout << "Websocket stopped.\n";
			running = false;
			return;
		}

		std::cout << "Unknown command: " << command << "\n";
	}

	void run() {
		running = true;

		while(running) {
			std::string command;
			std::cout << ">> ";
			std::cin >> command;

			processCommand(command);
		}
	}
};

int main(int argc, char **argv) {
	// $ ./Unofficial-Discord-Client [discord token]
	std::shared_ptr<Discord::Client> client = std::make_shared<Discord::Client>( (argc == 2)? argv[1] : "token", Discord::AuthTokenType::BOT);
	MyListener listener;
	
	client->AddListener(&listener);
	
	ConsoleTest console(client);

	std::thread consoleThread(&ConsoleTest::run, &console);

	client->Run();
	
	consoleThread.join();

	std::cout << "Exiting main with code 0\n";
	return 0;
}
