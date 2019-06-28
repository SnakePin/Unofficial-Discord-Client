#include <iostream>
#include <thread>

#include <discord/discord.hpp>
#include <discord/message.hpp>
#include <discord/guild.hpp>
#include <discord/packets.hpp>

#include <asio.hpp>
#include <asio/thread_pool.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/pointer.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>

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
		// explicitly do nothing!
		// this stops super->OnHelloPacket from running (and calling the SendIdentify method)
	}

	void OnReadyPacket(Discord::ReadyPacket packet) {
		std::cout << "Received ready packet: " << packet.version << " " << packet.sessionID << " " << packet.user.username << std::endl;
		std::cout << "Writing session data to session.json..." << std::endl;

		for(Discord::Guild &g : packet.guilds) {
			guilds.push_back(g);
		}

		UpdateSessionJson();
	}

	void OnGuildCreate(Discord::Guild g) {
		guilds.push_back(g);

		for(Discord::Channel &chan : g.channels) {
			if(chan.type == 0){
				Discord::MessagePacket messageToSend{ .content = "Hello to channel <#" + std::to_string(chan.id.value) + ">", .tts = false };
				asio::post(*pool,
					[=] {httpAPI.SendMessage(std::to_string(chan.id.value), messageToSend);}
				);
			}
		}

		UpdateSessionJson();
	}

	void OnTypingStart(Discord::TypingStartPacket packet) {
		if(packet.member)
			std::cout << packet.member.value().user.username << " is typing..." << std::endl;
	}

	void OnMessageCreate(Discord::Message m) {
		std::cout << m.id.value << " <" << m.author.username << "> " << m.content << std::endl;
	}

	void OnMessageReactionAdd(Discord::MessageReactionPacket p) {
		std::cout << p.messageID.value << " User " << p.userID.value << " added reaction " << p.emoji.name << std::endl;
	}

	void OnMessageReactionRemove(Discord::MessageReactionPacket p) {
		std::cout << p.messageID.value << " User " << p.userID.value << " removed reaction " << p.emoji.name << std::endl;
	}

	// Reads the session_id and sequence number from session.json and sends a RESUME packet with the read information.
	// This will do nothing if the session ID is too old.
	void LoadAndSendResume() {
		#if defined(_WIN32)
		FILE* fp = fopen("session.json", "r");
		#else
		FILE* fp = fopen("session.json", "rb"); // non-Windows use "r"
		#endif

		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		rapidjson::Document document;
		document.ParseStream(is);
		fclose(fp);

		std::time_t sessionTime = document["session_time"].GetInt();
		std::time_t currentTime = std::time(nullptr);

		if(currentTime - sessionTime < 10*60) {
			std::string sessionID = document["session_id"].GetString();
			int32_t sequence = document["seq"].GetInt();
			std::cout << "Resuming session with seq: " << sequence << std::endl;
			this->SendResume(sessionID, sequence);
		}else {
			std::cout << "The session has (possibly) expired. No resume was sent." << std::endl;
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
			client->UpdateSessionJson(); // writes the latest sequence number to the session.json file

			std::cout << "Stopping the websocket...\n";
			client->websocket.stop();
			std::cout << "Websocket stopped.\n";
			running = false;

		}
		else if(command == "identify") {
			std::cout << "Sending identify..." << std::endl;
			client->SendIdentify();

		}
		else if(command == "resume") {
			std::cout << "Sending resume..." << std::endl;
			client->LoadAndSendResume();

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
				std::cout << "\"" << guild.name << "\" ";
			}
			std::cout << "\n";
			
		}
		else if(command == "members") {
			// Loop through all the guilds and print their names

			for(Discord::Guild &guild : client->guilds) {
				std::cout << guild.name << " has " << guild.members.size() << " members." << std::endl;
				for(Discord::Member &m : guild.members) {
					std::cout << "    " << m.user.username << std::endl;
				}
				std::cout << "\n";
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
	std::shared_ptr<MyClient> client = std::make_shared<MyClient>( (argc == 2)? argv[1] : "token", Discord::AuthTokenType::USER);
	
	ConsoleTest console(client);

	std::thread consoleThread(&ConsoleTest::run, &console);

	client->Run();
	
	consoleThread.join();

	std::cout << "Exiting main with code 0\n";
	return 0;
}
