#include <iostream>
#include <thread>

#include <discord/discord.hpp>
#include <discord/guild.hpp>



class MyClient : public Discord::Client {
public:

	std::vector<Discord::Guild> guilds;

	MyClient(std::string token, Discord::AuthTokenType tokenType) : Client(token, tokenType) {

	}

	void OnGuildCreate(Discord::Guild g) {
		guilds.push_back(g);
	}
};

class ConsoleTest {
private:

	std::shared_ptr<MyClient> client;
	bool running;

public:
	ConsoleTest(std::shared_ptr<MyClient> client)
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
		else if(command.rfind("sendmsg", 0) == 0) {
			Discord::Message messageToSend;

			messageToSend.content = command.substr(command.find(' '));

			//This channel ID is channel ID of test discord guild's general channel's ID
			client->httpAPI.SendMessage("590695217028661250", messageToSend);
			return;
		}
		else if(command == "guilds") {
			// Loop through all the guilds and print their names

			std::cout << "Found " << client->guilds.size() << " guilds:\n    ";
			for(Discord::Guild &guild : client->guilds) {
				std::cout << "\"" << guild.name << "\" ";
			}
			std::cout << "\n";
			
			return;
		}

		std::cout << "Unknown command: " << command << "\n";
	}

	void run() {
		running = true;

		while(running) {
			std::string command;
			std::cout << ">> ";
			std::getline(std::cin, command);

			processCommand(command);
		}
	}
};

int main(int argc, char **argv) {
	// $ ./Unofficial-Discord-Client [discord token]
	std::shared_ptr<MyClient> client = std::make_shared<MyClient>( (argc == 2)? argv[1] : "token", Discord::AuthTokenType::BOT);
	
	ConsoleTest console(client);

	std::thread consoleThread(&ConsoleTest::run, &console);

	client->Run();
	
	consoleThread.join();

	std::cout << "Exiting main with code 0\n";
	return 0;
}
