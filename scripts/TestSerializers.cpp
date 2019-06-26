/*
    Compile as:
        $ g++ -I../include TestSerializers.cpp ../src/discord/serializers.cpp
    
    See GuildCreatePacket.json in this folder to see what it's parsing.
 */

#include <discord/guild.hpp>
#include <discord/member.hpp>
#include <discord/role.hpp>
#include <discord/message.hpp>

#include <iostream>
#include <fstream>
#include <ctime>

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

	std::cout << "Guild name: " << g.name.value() << "\n";
	std::cout << "Guild region: " << g.region.value() << "\n";
    std::cout << "Guild description: " << g.description.value_or("fuck") << "\n";
    std::cout << "Joined Guild at: " << g.joinedAt.value() << "\n";
    std::cout << "Guild Member Count: " << g.memberCount.value() << "\n";
    std::cout << "Guild ID: " << g.id.value << "\n";
    time_t guildCreateTime = g.id.UnixEpoch()/1000;
    std::cout << "Guild Epoch & ctime: " << g.id.UnixEpoch()/1000 << " " << ctime(&guildCreateTime) << "\n";

    std::cout << "Roles:\n";
    for(const Role &role : g.roles) {
        std::cout << "\tRole ID: " << role.id.value << "\n";
        std::cout << "\tRole name: " << role.name.value() << "\n\n";
    }

    std::cout << "Members:\n";

    for(const Member &mem : g.members) {
        std::cout << "\tID: " << mem.user.id.value << "\n";
        std::cout << "\tJoined at: " << mem.joinedAt.value() << "\n";
        std::cout << "\tUsername: " << mem.user.username.value() << "\n\n";
    }

    std::cout << "Channels:\n";
    std::string channelTypes[] = {"Text", "1", "Voice", "3", "Category"};
    for(const Channel &chan : g.channels) {
        std::cout << "\tType: " << channelTypes[chan.type.value()] << "\n";
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

    std::cout << "Message: " << m.content.value() << "\n";
	std::cout << "Author: " << m.author.value().username.value() << "\n";
    std::cout << "Message ID: " << m.id.value << "\n";
    time_t msgCreateTime = m.id.UnixEpoch()/1000;
    std::cout << "Message Epoch & ctime: " << m.id.UnixEpoch()/1000 << " " << ctime(&msgCreateTime) << "\n";
}

int main() {
	testGuildCreate();
	testMessageCreate();
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