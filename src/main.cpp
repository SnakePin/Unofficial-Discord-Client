#include <iostream>
#include <thread>

#include <discord/discord.hpp>
#include <discord/guild.hpp>

#include <asio.hpp>
#include <asio/thread_pool.hpp>

class MyClient : public Discord::Client {
public:

	std::shared_ptr<asio::thread_pool> pool; // user created threadpool for making HTTP requests
	std::vector<Discord::Guild> guilds;

	MyClient(std::string token, Discord::AuthTokenType tokenType)
	    : Client(token, tokenType),
		pool(std::make_shared<asio::thread_pool>(4)) /* Thread pool contains 4 threads. */ {

	}

	void OnGuildCreate(Discord::Guild g) {
		guilds.push_back(g);
		for(Discord::Channel &chan : g.channels)
			if(chan.type == 0){
				Discord::MessagePacket messageToSend{ .content = std::string("Hello to channel #" + g.name), .tts = false };
				asio::post(*pool,
					[=] {httpAPI.SendMessage(std::to_string(chan.id.value), messageToSend);}
				);
			}
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
		}
		else if(command.rfind("sendmsg", 0) == 0) {
			Discord::MessagePacket messageToSend{ .content = command.substr(command.find(' ')), .tts = false };

			//This channel ID is channel ID of test discord guild's general channel's ID
			asio::post(*client->pool, 
				[=] {
					client->httpAPI.SendMessage("590695217028661250", messageToSend);
				}
			);
		}
		else if(command.rfind("delaymsg", 0) == 0) {
			// Sends a 'typing' signal, then the message after a 5 second delay.
			Discord::MessagePacket messageToSend{ .content = command.substr(command.find(' ')), .tts = false };

			asio::post(*client->pool,
				[=] {
					client->httpAPI.StartTyping("590695217028661250");
					std::this_thread::sleep_for(std::chrono::seconds(5));
					client->httpAPI.SendMessage("590695217028661250", messageToSend);
				}
			);
		}
		else if(command == "guilds") {
			// Loop through all the guilds and print their names

			std::cout << "Found " << client->guilds.size() << " guilds:\n    ";
			for(Discord::Guild &guild : client->guilds) {
				std::cout << "\"" << guild.name << "\" ";
			}
			std::cout << "\n";
			
		}
		else{
			std::cout << "Unknown command: " << command << "\n";
		}
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
