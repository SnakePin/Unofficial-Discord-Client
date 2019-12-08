#include <iostream>
#include <thread>
#include <regex>
#include <variant>

#include "discord/discord.hpp"
#include "discord/message.hpp"
#include "discord/guild.hpp"
#include "discord/packets.hpp"
#include "discord/snowflake.hpp"
#include "discord/deserializers.hpp"

#include <asio.hpp>
#include <asio/thread_pool.hpp>

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

#include <rapidjson/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>

#include <tinyformat.h>

#include "imguiClient.hpp"
#include "myClient.hpp"

MyClient::MyClient(std::string& token, Discord::AuthTokenType tokenType)
	: Client(token, tokenType),
	pool(std::make_shared<asio::thread_pool>(4)), /* Thread pool contains 4 threads. */
	lastSessionUpdateTime(0)
{
	if (tokenType == Discord::AuthTokenType::USER) {
		std::cout << "** Client created in User mode **" << std::endl;
	}
}

void MyClient::UpdateSessionJson() {
	rapidjson::Document document;
	lastSessionUpdateTime = std::time(nullptr);

	rapidjson::Pointer("/session_id").Set(document, sessionID.c_str());
	rapidjson::Pointer("/seq").Set(document, this->sequenceNumber);
	rapidjson::Pointer("/session_time").Set(document, lastSessionUpdateTime);

	//TODO: implement template based serializers for every struct and most of the C types too and use them here
	//rapidjson::Pointer("/guilds").Set(document, guilds);
	//rapidjson::Pointer("/private_channels").Set(document, privateChannels);

	std::ofstream jsonFile("session.json");
	rapidjson::OStreamWrapper outstreamwrapper(jsonFile);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(outstreamwrapper);
	document.Accept(writer);
}

void MyClient::StopAndSaveSession() {
	UpdateSessionJson();
	Stop();
}

void MyClient::OnWSSError(SimpleWeb::error_code errorCode) {
	isIdentified = false;
}

void MyClient::OnWSSDisconnect(int statusCode, std::string reason) {
	isIdentified = false;
}

void MyClient::OnHelloPacket() {
	// explicitly do nothing!
	// this stops super->OnHelloPacket from running (and calling the SendIdentify method)
}

void MyClient::OnReadyPacket(Discord::ReadyPacket packet) {
	std::cout << "Received ready packet: " << packet.version << " " << packet.sessionID << " " << packet.user.username << std::endl;
	isIdentified = true;
	// TODO shorten this

	// Only add Guilds whose IDs we don't have.
	{
		guildsVectorMutex.lock();
		std::vector<Discord::Snowflake> existing(guilds.size());
		std::transform(guilds.begin(), guilds.end(), existing.begin(), [](const Discord::Guild& g){ return g.id; });

		std::copy_if(packet.guilds.begin(), packet.guilds.end(), std::back_inserter(guilds), [&](const Discord::Guild& g){
			return std::find(existing.begin(), existing.end(), g.id) == existing.end();
		});
		guildsVectorMutex.unlock();
	}

	// Ditto, private channels
	{
		privateChannelsVectorMutex.lock();
		std::vector<Discord::Snowflake> existing(privateChannels.size());
		std::transform(privateChannels.begin(), privateChannels.end(), existing.begin(), [](const Discord::Channel& c){ return c.id; });

		std::copy_if(packet.privateChannels.begin(), packet.privateChannels.end(), std::back_inserter(privateChannels), [&](const Discord::Channel& c){
			return std::find(existing.begin(), existing.end(), c.id) == existing.end();
		});
		privateChannelsVectorMutex.unlock();
	}

	std::cout << "Writing session data to session.json..." << std::endl;
	UpdateSessionJson();
}

void MyClient::OnGuildCreate(Discord::Guild g) {
	guildsVectorMutex.lock();
	guilds.push_back(g);
	guildsVectorMutex.unlock();
}

void MyClient::OnGuildMemberListUpdate(Discord::GuildMemberListUpdatePacket packet) {
	using namespace Discord;
	std::cout << "Received: GuildMemberListUpdatePacket\nGuild ID: " << packet.guildID.value << std::endl;
	std::cout << "Groups: ";
	for(const GuildMemberListGroup &group : packet.groups) std::cout << group.id << "(" << group.count << "), ";
	std::cout << std::endl;

	std::cout << "We have these operations:" << std::endl;
	for(const GuildMemberListUpdateOperation& operation : packet.operations) {
		std::cout << "\t" << "OP: " << operation.op << std::endl;
		std::cout << "\t" << "Range: " << operation.range.first << " - " << operation.range.second << std::endl;

		for(const std::variant<GuildMemberListGroup, Member>& item : operation.items) {
			if(std::holds_alternative<GuildMemberListGroup>(item)) {
				GuildMemberListGroup group = std::get<GuildMemberListGroup>(item);
				std::cout << "\t\t" << group.id << " - " << group.count << std::endl;

			}else if(std::holds_alternative<Member>(item)) {
				Member member = std::get<Member>(item);
				std::cout << "\t\t" << "  Member: " << member.user.username << std::endl;
			}
		}
	}
}

void MyClient::OnTypingStart(Discord::TypingStartPacket packet) {
	if(packet.member)
		std::cout << packet.member.value().user.username << " is typing..." << std::endl;
}

void MyClient::OnMessageCreate(Discord::Message m) {
	std::cout << m.id.value << " <" << m.author.username << "> " << m.content << std::endl;
}

void MyClient::OnMessageReactionAdd(Discord::MessageReactionPacket p) {
	std::cout << p.messageID.value << " User " << p.userID.value << " added reaction " << p.emoji.name << std::endl;
}

void MyClient::OnMessageReactionRemove(Discord::MessageReactionPacket p) {
	std::cout << p.messageID.value << " User " << p.userID.value << " removed reaction " << p.emoji.name << std::endl;
}

void MyClient::LoadAndSendResume() {
	std::ifstream jsonFile("session.json");
	rapidjson::IStreamWrapper iswrapper(jsonFile);
	rapidjson::Document document;
	document.ParseStream(iswrapper);

	std::time_t sessionTime = document["session_time"].GetInt();
	std::time_t currentTime = std::time(nullptr);

	if(currentTime - sessionTime < 10*60) {
		std::string sessionID = document["session_id"].GetString();
		int32_t sequence = document["seq"].GetInt();
		std::cout << "Resuming session with seq: " << sequence << std::endl;
		SendResume(sessionID, sequence);
		//JsonTypeToStructType(document, "guilds", guilds);
		//JsonTypeToStructType(document, "private_channels", privateChannels);
	}else {
		std::cout << "The session has (possibly) expired. No resume was sent." << std::endl;
	}
}

class ConsoleTest {
private:

	std::shared_ptr<MyClient> client;
	bool running;
	Discord::Snowflake channelToOperateOn;

public:
	ConsoleTest(std::shared_ptr<MyClient> client)
		: client(client), running(false), channelToOperateOn(590695217028661250) //This is the internal developers test channel
	{

	}

	void processCommand(std::string command) {
		if(command == "quit") {
			std::cout << "Stopping..." << std::endl;
			client->StopAndSaveSession();
			std::cout << "Stopped." << std::endl;
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
			Discord::CreateMessageParam messageToSend;
			messageToSend.content = command.substr(command.find(' '));
			messageToSend.tts = false;

			//This channel ID is channel ID of test discord guild's general channel's ID
			//TODO: remove this id
			asio::post(*client->pool, 
				[this, messageToSend] {
					client->httpAPI.CreateMessage(channelToOperateOn, messageToSend);
				}
			);
			
		}
		else if(command.rfind("messages", 0) == 0) {
			Discord::Snowflake channelID(command.substr(command.find(' ')));

			asio::post(*client->pool, 
				[this, channelID] {
					std::vector<Discord::Message> newMessages;
					std::cout << "Requesting 50 messages from channel " << channelID.value << "..." << std::endl;

					client->httpAPI.GetMessagesInto(channelToOperateOn, newMessages);

					std::cout << "Got " << newMessages.size() << " new messages." << std::endl;
					for(const auto& message : newMessages) {
						tfm::printf("<%s> %s\n", message.author.username, message.content);
					}
					std::cout << "--------------------------" << std::endl;
				}
			);

		}
		else if(command.rfind("delaymsg", 0) == 0) {
			// Sends a 'typing' signal, then the message after a 5 second delay.
			Discord::CreateMessageParam messageToSend;
			messageToSend.content = command.substr(command.find(' '));
			messageToSend.tts = false;

			asio::post(*client->pool,
				[this, messageToSend] {
					client->httpAPI.TriggerTypingIndicator(channelToOperateOn);
					std::this_thread::sleep_for(std::chrono::seconds(5));
					client->httpAPI.CreateMessage(channelToOperateOn, messageToSend);
				}
			);

		}
		else if(command == "guilds") {
			// Loop through all the guilds and print their names

			client->guildsVectorMutex.lock();
			auto guildsVectorCopy = client->guilds;
			client->guildsVectorMutex.unlock();
			std::cout << "Found " << guildsVectorCopy.size() << " guilds:\n\t";
			for(Discord::Guild &guild : guildsVectorCopy) {
				std::cout << guild.name << " | " << guild.id.value << std::endl;
				for(Discord::Channel &channel : guild.channels) {
					std::cout << "\tType " << channel.type << " | " << channel.name.value_or("(Channel name?)") << " | " << channel.id.value << std::endl;
				}
				std::cout << std::endl;
				std::cout << std::endl;
			}

			client->privateChannelsVectorMutex.lock();
			auto privateChannelsCopy = client->privateChannels;
			client->privateChannelsVectorMutex.unlock();
			std::cout << "Found " << privateChannelsCopy.size() << " private channels:" << std::endl;
			for(Discord::Channel &chan : privateChannelsCopy) {
				std::cout << "Type: " << chan.type << " | " << chan.name.value_or("(Noname)") << " | " << chan.id.value << std::endl;
				std::cout << std::endl;
			}
			
		}
		else if(command == "members") {
			// Loop through all the guilds and print their names

			for(Discord::Guild &guild : client->guilds) {
				std::cout << guild.name << " has " << guild.members.size() << " members." << std::endl;
				for(Discord::Member &m : guild.members) {
					std::cout << "\t" << m.user.username << std::endl;
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			
		}
		else if(command.rfind("switch", 0) == 0) {

			static std::regex singleArgument("^switch \\d+$");
			static std::regex doubleArgument("^switch \\d+ \\d+$");

			// Single argument indicates switching to a private message channel
			if(std::regex_match(command, singleArgument)) {
				std::smatch match;
				std::regex_search(command, match, std::regex("\\d+"));
				Discord::Snowflake channelID(match.str());

				std::cout << "Switching to private channel: " << channelID.value << " (OP13) ..." << std::endl;
				channelToOperateOn = channelID;
				
				// TODO this is non functional (!??!!)
				// client->OpenPrivateChannelView(channelID);

			}
			// Two argument indicates switching to a GUILD and CHANNEL
			else if(std::regex_match(command, doubleArgument)) {
				std::smatch match;
				
				std::regex_search(command, match, std::regex("\\d+"));
				Discord::Snowflake guildID(match.str());
				command = match.suffix().str();

				std::regex_search(command, match, std::regex("\\d+"));
				Discord::Snowflake channelID(match.str());

				std::cout << "Switching to Guild/Channel: " << guildID.value << "/" << channelID.value << " (OP14) ..." << std::endl;
				client->OpenGuildChannelView(guildID, channelID);

				channelToOperateOn = channelID;
			}
			else {
				goto UNKNOWN_COMMAND; // haHaa!
			}

		}
		else if(command.rfind("setstatus", 0) == 0) {
				std::string status = command.substr(command.find(' '));
				if(status == "online" || status == "idle" || status == "dnd") {
						client->UpdatePresence(status);
				} else {
						std::cout << "This is not a valid status" << std::endl;
						return;
				}
		}
		else{
			UNKNOWN_COMMAND:
			std::cout << "Unknown command: " << command << std::endl;
		}
	}

	void stop() {
		running = false;
	}

	void run() {
		running = true;

		while(running) {
			std::string command;
			std::cout << ">> ";
			if (!running) { break; }
			std::getline(std::cin, command);
			processCommand(command);
		}
	}
};

void showHelpMessage(char* firstArg)
{
	std::cout << "Usage: " << std::endl
	<< firstArg << " <Discord token> [token type]" << std::endl
	<< "Info: Token type can be either \"USER\" or \"BOT\", it defaults to \"USER\" if not specified.";
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		showHelpMessage(argv[0]);
		return 1;
	}

	Discord::AuthTokenType tokenType = Discord::AuthTokenType::USER;
	if(argc >= 3) {
		if(strcmp(argv[2], "USER") == 0) {
			tokenType = Discord::AuthTokenType::USER;
		} else if(strcmp(argv[2], "BOT") == 0) {
			tokenType = Discord::AuthTokenType::BOT;
		}
	}

	std::string tokenString(argv[1]);

	std::shared_ptr<MyClient> client = std::make_shared<MyClient>(tokenString, tokenType);
	std::shared_ptr<std::thread> clientThread = std::make_shared<std::thread>(&Discord::Client::Run, &*client);

	//TODO: make ConsoleTest better and thread-safe, add "condition_variable"s etc
	//ConsoleTest console(client);
	//std::thread consoleThread(&ConsoleTest::run, &console);
	
	std::thread uiThread(&startImguiClient, client, clientThread);
	
	uiThread.join();
	//console.stop();
	//consoleThread.join();

	client->StopAndSaveSession();
	clientThread->join();

	std::cout << "Exiting main with code 0" << std::endl;
	return 0;
}
