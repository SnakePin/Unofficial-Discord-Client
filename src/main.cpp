#include <iostream>
#include <thread>

#include <discord/discord.hpp>
#include <discord/guild.hpp>
#include <discord/packets.hpp>

#include <asio.hpp>
#include <asio/thread_pool.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/pointer.h>
#include <rapidjson/filewritestream.h>

class MyClient : public Discord::Client {
public:

	std::shared_ptr<asio::thread_pool> pool; // user created threadpool for making HTTP requests
	std::vector<Discord::Guild> guilds;
	std::time_t lastSessionUpdateTime;

	MyClient(std::string token, Discord::AuthTokenType tokenType)
	    : Client(token, tokenType),
		pool(std::make_shared<asio::thread_pool>(4)), /* Thread pool contains 4 threads. */
		lastSessionUpdateTime(0) {

	}

	// Writes the session ID, sequence number, and update time to session.json.
	void UpdateSessionJson() {
		rapidjson::Document document;
		lastSessionUpdateTime = std::time(nullptr);

		rapidjson::Pointer("/session_id").Set(document, sessionID.c_str());
		rapidjson::Pointer("/seq").Set(document, this->sequenceNumber);
		rapidjson::Pointer("/session_time").Set(document, lastSessionUpdateTime);

		#if defined(_WIN32)
		FILE* fp = fopen("session.json", "w");
		#else
		FILE* fp = fopen("session.json", "wb");
		#endif

		char writeBuffer[65536];
		rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

		rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
		document.Accept(writer);

		fclose(fp);
	}

	void OnHelloPacket() {
		std::cout << "Sending identify..." << std::endl;
		SendIdentify();
	}

	void OnReadyPacket(Discord::ReadyPacket packet) {
		std::cout << "Received ready packet: " << packet.version << " " << packet.sessionID << " " << packet.user.username.value_or("") << std::endl;
		std::cout << "Writing session data to session.json..." << std::endl;

		UpdateSessionJson();
	}

	void OnGuildCreate(Discord::Guild g) {
		guilds.push_back(g);

		for(Discord::Channel &chan : g.channels) {
			if(chan.type.value() == 0){
				Discord::MessagePacket messageToSend{ .content = "Hello to channel <#" + std::to_string(chan.id.value) + ">", .tts = false };
				asio::post(*pool,
					[=] {httpAPI.SendMessage(std::to_string(chan.id.value), messageToSend);}
				);
			}
		}

		UpdateSessionJson();
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
					client->httpAPI.SendMessage(Discord::Snowflake(590695217028661250), messageToSend);
				}
			);
		}
		else if(command.rfind("delaymsg", 0) == 0) {
			// Sends a 'typing' signal, then the message after a 5 second delay.
			Discord::MessagePacket messageToSend{ .content = command.substr(command.find(' ')), .tts = false };

			asio::post(*client->pool,
				[=] {
					client->httpAPI.StartTyping(Discord::Snowflake(590695217028661250));
					std::this_thread::sleep_for(std::chrono::seconds(5));
					client->httpAPI.SendMessage(Discord::Snowflake(590695217028661250), messageToSend);
				}
			);
		}
		else if(command == "guilds") {
			// Loop through all the guilds and print their names

			std::cout << "Found " << client->guilds.size() << " guilds:\n    ";
			for(Discord::Guild &guild : client->guilds) {
				std::cout << "\"" << guild.name.value() << "\" ";
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
