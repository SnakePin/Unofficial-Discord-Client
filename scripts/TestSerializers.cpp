/*
    Compile as:
        $ g++ -std=c++17 -I../include TestSerializers.cpp ../src/discord/serializers.cpp
    
    See GuildCreatePacket.json in this folder to see what it's parsing.
 */

#include <discord/guild.hpp>
#include <discord/member.hpp>
#include <discord/role.hpp>
#include <discord/message.hpp>

#include <discord/packets.hpp>

#include <iostream>
#include <fstream>
#include <ctime>
#include <variant>

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

static char * ReadAllBytes(const char * filename, int * read);

void testGuildCreate() {
	using namespace Discord;

	int read;
	char *json = ReadAllBytes("GuildCreatePacket.json", &read);
	rapidjson::Document doc;
	doc.Parse(json);
    delete[] json;

	Guild g = Guild::LoadFrom(doc, "/d");

	std::cout << "Guild name: " << g.name << "\n";
	std::cout << "Guild region: " << g.region << "\n";
    std::cout << "Guild description: " << g.description << "\n";
    std::cout << "Joined Guild at: " << g.joinedAt.value() << "\n";
    std::cout << "Guild Member Count: " << g.memberCount.value_or(0) << "\n";
    std::cout << "Guild ID: " << g.id.value << "\n";
    time_t guildCreateTime = g.id.UnixEpoch()/1000;
    std::cout << "Guild Epoch & ctime: " << g.id.UnixEpoch()/1000 << " " << ctime(&guildCreateTime) << "\n";

    std::cout << "Roles:\n";
    for(const Role &role : g.roles) {
        std::cout << "\tRole ID: " << role.id.value << "\n";
        std::cout << "\tRole name: " << role.name << "\n\n";
    }

    std::cout << "Members:\n";

    for(const Member &mem : g.members) {
        std::cout << "\tID: " << mem.user.id.value << "\n";
        std::cout << "\tJoined at: " << mem.joinedAt << "\n";
        std::cout << "\tUsername: " << mem.user.username << "\n\n";
    }

    std::cout << "Channels:\n";
    std::string channelTypes[] = {"Text", "1", "Voice", "3", "Category"};
    for(const Channel &chan : g.channels) {
        std::cout << "\tType: " << channelTypes[chan.type] << "\n";
        std::cout << "\tName: " << chan.name.value() << "\n\n";
    }
}

void testMessageCreate() {
	using namespace Discord;

	int read;
	char *json = ReadAllBytes("MessageCreatePacket.json", &read);
	rapidjson::Document doc;
	doc.Parse(json);
    delete[] json;

	Message m = Message::LoadFrom(doc, "/d");

    std::cout << "Message: " << m.content << "\n";
	std::cout << "Author: " << m.author.username << "\n";
    std::cout << "Message ID: " << m.id.value << "\n";
    time_t msgCreateTime = m.id.UnixEpoch()/1000;
    std::cout << "Message Epoch & ctime: " << m.id.UnixEpoch()/1000 << " " << ctime(&msgCreateTime) << "\n";
}

void testTypingStart() {
	using namespace Discord;

	int read;
	char *json = ReadAllBytes("TypingStartPacket.json", &read);
	rapidjson::Document doc;
	doc.Parse(json);
    delete[] json;

	TypingStartPacket p = TypingStartPacket::LoadFrom(doc, "/d");

    std::cout << "User: " << p.userID.value << "\n";
	std::cout << "Channel: " << p.channelID.value << "\n";
	std::cout << "Channel: " << p.guildID.value().value << "\n";
    time_t msgCreateTime = p.timestamp;
    std::cout << "Packet ctime: " << ctime(&msgCreateTime) << "\n";
}

void testReactionAdd() {
	using namespace Discord;

	int read;
	char *json = ReadAllBytes("MessageReactionAddPacket.json", &read);
	rapidjson::Document doc;
	doc.Parse(json);
    delete[] json;

	MessageReactionPacket p = MessageReactionPacket::LoadFrom(doc, "/d");
	Emoji &emote = p.emoji;

    std::cout << "User: " << p.userID.value << "\n";
	std::cout << "Channel: " << p.channelID.value << "\n";
	std::cout << "Guild: " << p.guildID.value_or( Snowflake() ).value << "\n";
	std::cout << "Message: " << p.messageID.value << "\n";
	std::cout << "Emoji: " << emote.name << " (size " << emote.name.size() << ")" << std::endl;
	std::cout << "Emoji ID: " << emote.id.value << std::endl;
}

void testGuildMemberListUpdate() {
	using namespace Discord;

	int read;
	char *json = ReadAllBytes("GuildMemberListUpdatePacket.json", &read);
	rapidjson::Document doc;
	doc.Parse(json);
    delete[] json;

	GuildMemberListUpdatePacket packet = GuildMemberListUpdatePacket::LoadFrom(doc, "/d");
	std::cout << "GuildMemberListUpdatePacket\nGuild ID: " << packet.guildID.value << std::endl;
	std::cout << "We have these groups: ";
	for(const GuildMemberListGroup &group : packet.groups) std::cout << group.id << "(" << group.count << "), ";
	std::cout << std::endl;

	std::cout << "We have these operations:" << std::endl;
	for(const GuildMemberListUpdateOperation& operation : packet.operations) {
		std::cout << "    " << "OP: " << operation.op << std::endl;
		std::cout << "    " << "Range: " << operation.range.first << " - " << operation.range.second << std::endl;

		for(const std::variant<GuildMemberListGroup, Member>& item : operation.items) {
			if(std::holds_alternative<GuildMemberListGroup>(item)) {
				GuildMemberListGroup group = std::get<GuildMemberListGroup>(item);
				std::cout << "        " << group.id << " - " << group.count << std::endl;

			}else if(std::holds_alternative<Member>(item)) {
				Member member = std::get<Member>(item);
				std::cout << "        " << "  Member: " << member.user.username << std::endl;
			}
		}
	}
}

int main() {
	// testGuildCreate();
	// testMessageCreate();
	// testTypingStart();
	// testReactionAdd();
	testGuildMemberListUpdate();
	return 0;
}

char *  ReadAllBytes(const char * filename, int * read) {
	using namespace std;
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    int length = pos;

    // Manuall memory management.
    // Not a good idea use a container/.
    char *pChars = new char[length];
    ifs.seekg(0, ios::beg);
    ifs.read(pChars, length);

    ifs.close();
    *read = length;
    return pChars;
}