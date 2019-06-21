/*
    Compile as:
        $ g++ -I../include TestSerializers.cpp ../src/discord/serializers.cpp
    
    See GuildCreatePacket.json in this folder to see what it's parsing.
 */

#include <discord/guild.hpp>
#include <discord/member.hpp>
#include <discord/role.hpp>

#include <iostream>
#include <fstream>
#include <ctime>

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

static char * ReadAllBytes(const char * filename, int * read);

int main() {
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
    std::cout << "Joined Guild at: " << g.joinedAt << "\n";
    std::cout << "Guild Member Count: " << g.memberCount << "\n";
    std::cout << "Guild ID: " << g.id.value << "\n";
    time_t guildCreateTime = g.id.UnixEpoch()/1000;
    std::cout << "Guild Epoch & ctime: " << g.id.UnixEpoch()/1000 << " " << ctime(&guildCreateTime) << "\n\n";

    std::cout << "Roles:\n";
    for(const Role &role : g.roles) {
        std::cout << "\tRole ID: " << role.id.value << "\n";
        std::cout << "\tRole name: " << role.name << "\n\n";
    }

    std::cout << "\nMembers:\n";

    for(const Member &mem : g.members) {
        std::cout << "\tID: " << mem.user.id.value << "\n";
        std::cout << "\tJoined at: " << mem.joinedAt << "\n";
        std::cout << "\tUsername: " << mem.user.username << "\n\n";
    }
    std::cout << "\n";

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