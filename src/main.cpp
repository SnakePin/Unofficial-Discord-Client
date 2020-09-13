#include <iostream>
#include <thread>
#include <regex>
#include <variant>

#include "discord/discord.hpp"
#include "discord/message.hpp"
#include "discord/guild.hpp"
#include "discord/packets.hpp"
#include "discord/snowflake.hpp"
#include "discord/serializers.hpp"

#include <tclap/CmdLine.h>
#include <string>

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

	rapidjson::Pointer("/session_id").Set(document, sessionID);
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

void MyClient::OnPostStop() {
	UpdateSessionJson();
	isIdentified = false;
}

void MyClient::OnResumeSuccess() {
	isIdentified = true;
}

void MyClient::OnHelloPacket() {
	// explicitly do nothing!
	// this stops super->OnHelloPacket from running (and calling the SendIdentify method)
}

void MyClient::OnReadyPacket(Discord::ReadyPacket packet) {
	std::cout << "Received ready packet:" << std::endl <<
		"\tpacket.version: " << packet.version << std::endl <<
		"\tpacket.sessionID: " << packet.sessionID << std::endl <<
		"\tpacket.user.username: " << packet.user.username << std::endl <<
		"\tpacket.user.discriminator: " << packet.user.discriminator << std::endl;
	isIdentified = true;
	// TODO shorten this

	// Only add Guilds whose IDs we don't have.
	{
		guildsVectorMutex.lock();
		std::vector<Discord::Snowflake> existing(guilds.size());
		std::transform(guilds.begin(), guilds.end(), existing.begin(), [](const Discord::Guild& g) { return g.id; });

		std::copy_if(packet.guilds.begin(), packet.guilds.end(), std::back_inserter(guilds), [&](const Discord::Guild& g) {
			return std::find(existing.begin(), existing.end(), g.id) == existing.end();
		});
		guildsVectorMutex.unlock();
	}

	// Ditto, private channels
	{
		privateChannelsVectorMutex.lock();
		std::vector<Discord::Snowflake> existing(privateChannels.size());
		std::transform(privateChannels.begin(), privateChannels.end(), existing.begin(), [](const Discord::Channel& c) { return c.id; });

		std::copy_if(packet.privateChannels.begin(), packet.privateChannels.end(), std::back_inserter(privateChannels), [&](const Discord::Channel& c) {
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
	for (const GuildMemberListGroup& group : packet.groups) std::cout << group.id << "(" << group.count << "), ";
	std::cout << std::endl;

	std::cout << "We have these operations:" << std::endl;
	for (const GuildMemberListUpdateOperation& operation : packet.operations) {
		std::cout << "\t" << "OP: " << operation.op << std::endl;
		std::cout << "\t" << "Range: " << operation.range.first << " - " << operation.range.second << std::endl;

		for (const std::variant<GuildMemberListGroup, Member>& item : operation.items) {
			if (std::holds_alternative<GuildMemberListGroup>(item)) {
				GuildMemberListGroup group = std::get<GuildMemberListGroup>(item);
				std::cout << "\t\t" << group.id << " - " << group.count << std::endl;

			}
			else if (std::holds_alternative<Member>(item)) {
				Member member = std::get<Member>(item);
				std::cout << "\t\t" << "  Member: " << member.user.username << std::endl;
			}
		}
	}
}

void MyClient::OnTypingStart(Discord::TypingStartPacket packet) {
	if (packet.member)
		std::cout << packet.member.value().user.username << " is typing..." << std::endl;
}

void MyClient::OnMessageCreate(Discord::Message m) {
	std::cout << m.id.value << " <" << m.author.username << "> " << m.content << std::endl;
}

void MyClient::OnMessageReactionAdd(Discord::MessageReactionPacket p) {

	std::cout << p.messageID.value << " User " << p.userID.value << " added reaction ";

	if (p.emoji.name.has_value()) {
		std::cout << p.emoji.name.value() << std::endl;
	}
	else {
		std::cout << "(Unnamed emoji)" << std::endl;
	}
}

void MyClient::OnMessageReactionRemove(Discord::MessageReactionPacket p) {
	std::cout << p.messageID.value << " User " << p.userID.value << " removed reaction ";

	if (p.emoji.name.has_value()) {
		std::cout << p.emoji.name.value() << std::endl;
	}
	else {
		std::cout << "(Unnamed emoji)" << std::endl;
	}
}

void MyClient::LoadAndSendResume() {
	std::ifstream jsonFile("session.json");

	if(!jsonFile.good()) {
		std::cout << "session.json can not be read!" << std::endl;
		return;
	}

	rapidjson::IStreamWrapper iswrapper(jsonFile);
	rapidjson::Document document;
	document.ParseStream(iswrapper);

	std::time_t sessionTime = document["session_time"].GetInt();
	std::time_t currentTime = std::time(nullptr);

	if ((currentTime - sessionTime) < (10 * 60)) {
		std::string sessionID_local = document["session_id"].GetString();
		int32_t sequenceNumber_local = document["seq"].GetInt();
		std::cout << "Resuming session with seq: " << sequenceNumber_local << std::endl;
		SendResume(sessionID_local, sequenceNumber_local);
		//JsonTypeToStructType(document, "guilds", guilds);
		//JsonTypeToStructType(document, "private_channels", privateChannels);
	}
	else {
		std::cout << "The session has (possibly) expired. No resume was sent." << std::endl;
	}
}

class ConsoleTest {
private:

	std::shared_ptr<MyClient> client;
	std::atomic<bool> running;
	Discord::Snowflake channelToOperateOn;

public:
	ConsoleTest(std::shared_ptr<MyClient> client)
		: client(client), running(false), channelToOperateOn(590695217028661250) //This is the internal developers test channel
	{

	}

	void processCommand(std::string command) {
		if (command == "quit") {
			std::cout << "Stopping..." << std::endl;
			client->StopAndSaveSession();
			running = false;
		}
		else if (command == "help" || command == "?") {
			std::cout << "List of available commands: quit, help, ?, identify, resume, sendmsg, messages, delaymsg, guilds, members, switch, setstatus." << std::endl;
		}
		else if (command == "identify") {
			std::cout << "Sending identify..." << std::endl;
			client->SendIdentify();
		}
		else if (command == "resume") {
			std::cout << "Sending resume..." << std::endl;
			client->LoadAndSendResume();
		}
		else if (command.rfind("sendmsg", 0) == 0) {
			Discord::CreateMessageParam messageToSend;
			messageToSend.content = command.substr(command.find(' '));
			messageToSend.tts = false;

			asio::post(*client->pool,
				[this, messageToSend] {
				client->httpAPI.CreateMessage(channelToOperateOn, messageToSend);
			}
			);
		}
		else if (command.rfind("messages", 0) == 0) {
			Discord::Snowflake channelID(command.substr(command.find(' ')));

			asio::post(*client->pool,
				[this, channelID] {
				std::vector<Discord::Message> newMessages;
				std::cout << "Requesting 50 messages from channel " << channelID.value << "..." << std::endl;

				client->httpAPI.GetMessagesInto(channelToOperateOn, newMessages);

				std::cout << "Got " << newMessages.size() << " new messages." << std::endl;
				for (const auto& message : newMessages) {
					tfm::printf("<%s> %s\n", message.author.username, message.content);
				}
				std::cout << "--------------------------" << std::endl;
			}
			);

		}
		else if (command.rfind("delaymsg", 0) == 0) {
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
		else if (command == "guilds") {
			// Loop through all the guilds and print their names

			client->guildsVectorMutex.lock();
			auto guildsVectorCopy = client->guilds;
			client->guildsVectorMutex.unlock();
			std::cout << "Found " << guildsVectorCopy.size() << " guilds:\n\t";
			for (Discord::Guild& guild : guildsVectorCopy) {
				std::cout << guild.name << " | " << guild.id.value << std::endl;
				for (Discord::Channel& channel : guild.channels) {
					std::cout << "\tType " << channel.type << " | " << channel.name.value_or("(Channel name?)") << " | " << channel.id.value << std::endl;
				}
				std::cout << std::endl;
				std::cout << std::endl;
			}

			client->privateChannelsVectorMutex.lock();
			auto privateChannelsCopy = client->privateChannels;
			client->privateChannelsVectorMutex.unlock();
			std::cout << "Found " << privateChannelsCopy.size() << " private channels:" << std::endl;
			for (Discord::Channel& chan : privateChannelsCopy) {
				std::cout << "Type: " << chan.type << " | " << chan.name.value_or("(Noname)") << " | " << chan.id.value << std::endl;
				std::cout << std::endl;
			}

		}
		else if (command == "members") {
			// Loop through all the guilds and print their names

			for (Discord::Guild& guild : client->guilds) {
				std::cout << guild.name << " has " << guild.members.size() << " members." << std::endl;
				for (Discord::Member& m : guild.members) {
					std::cout << "\t" << m.user.username << std::endl;
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;

		}
		else if (command.rfind("switch", 0) == 0) {

			static std::regex singleArgument("^switch \\d+$");
			static std::regex doubleArgument("^switch \\d+ \\d+$");

			// Single argument indicates switching to a private message channel
			if (std::regex_match(command, singleArgument)) {
				std::smatch match;
				std::regex_search(command, match, std::regex("\\d+"));
				Discord::Snowflake channelID(match.str());

				std::cout << "Switching to private channel: " << channelID.value << " (OP13) ..." << std::endl;
				channelToOperateOn = channelID;

				// TODO this is non functional (!??!!)
				// client->OpenPrivateChannelView(channelID);

			}
			// Two argument indicates switching to a GUILD and CHANNEL
			else if (std::regex_match(command, doubleArgument)) {
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
		else if (command.rfind("setstatus", 0) == 0) {
			std::string status = command.substr(command.find(' '));
			if (status == "online" || status == "idle" || status == "dnd") {
				client->UpdatePresence(status);
			}
			else {
				std::cout << "This is not a valid status, valid statuses are \"online\", \"idle\" and \"dnd\"" << std::endl;
				return;
			}
		}
		else {
		UNKNOWN_COMMAND:
			std::cout << "Unknown command: \"" << command << "\". Type \"help\" or \"?\" to get a list of available commands." << std::endl;
		}
	}

	void Stop() {
		running = false;
	}

	void Run() {
		if(running) {
			return;
		}

		running = true;

		while (running) {
			std::string command;
			std::cout << ">> ";
			std::getline(std::cin, command);
			processCommand(command);
		}
	}
};

int main(int argc, char* argv[]) {

	//TODO: don't enforce token parameter on GUI

	TCLAP::CmdLine cmd("Unofficial Discord Client (UDC)");

	TCLAP::ValueArg<std::string> discordTokenArg("t", "token", "Discord token", true, "xyz12345", "discord token");
	TCLAP::SwitchArg showGUIArg("g", "gui", "Show GUI", false);
	TCLAP::SwitchArg showConsoleArg("c", "console", "Show console", false);
	TCLAP::ValueArg<std::string> discordTokenTypeArg("", "tokentype", "The type of the discord token, can be either \"user\" or \"bot\".", false, "user", "discord token type");
	cmd.add(showGUIArg);
	cmd.add(showConsoleArg);
	cmd.add(discordTokenArg);
	cmd.add(discordTokenTypeArg);

	try {
		cmd.parse(argc, argv);
	}
	catch (TCLAP::ArgException & e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}

	if (!showGUIArg.getValue() && !showConsoleArg.getValue()) {
		std::cerr << "Error: Please provide at least either the argument to show the console or the GUI." << std::endl;
		return 1;
	}

	Discord::AuthTokenType tokenType = Discord::AuthTokenType::USER;
	if (discordTokenTypeArg.isSet()) {
		if (discordTokenTypeArg.getValue() == "user") {
			tokenType = Discord::AuthTokenType::USER;
		}
		else if (discordTokenTypeArg.getValue() == "bot") {
			tokenType = Discord::AuthTokenType::BOT;
		}
	}

	std::string tokenString(discordTokenArg.getValue());

	std::shared_ptr<MyClient> client = std::make_shared<MyClient>(tokenString, tokenType);
	
	client->Run();

	//TODO: make ConsoleTest better and thread-safe

	std::optional<std::thread> consoleThread;
	std::optional<std::thread> uiThread;

	if (showConsoleArg.getValue()) {
		std::cout << "Starting console client." << std::endl;
		ConsoleTest console(client);
		consoleThread = std::thread(&ConsoleTest::Run, &console);
	}

	if (showGUIArg.getValue()) {
		std::cout << "Starting GUI client." << std::endl;
		uiThread = std::thread(&startImguiClient, client);
	}

	if (consoleThread.has_value()) {
		consoleThread.value().join();
	}
	if (uiThread.has_value()) {
		uiThread.value().join();
	}

	client->StopAndSaveSession();

	std::cout << "Exiting main with code 0" << std::endl;
	return 0;
}
