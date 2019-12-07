#pragma once

#include "discord/user.hpp"
#include "discord/emoji.hpp"
#include "discord/guild.hpp"
#include "discord/channel.hpp"
#include "discord/deserializable_serializable_class_type.hpp"

#include <vector>
#include <optional>
#include <variant>
#include <functional>

namespace Discord {

	struct GuildMemberListGroup : Deserializable_Serializable_Class<GuildMemberListGroup>  {
		std::string id; // NOT a Snowflake
		int32_t count;

		static GuildMemberListGroup LoadFrom(rapidjson::Document &doc, std::string pointer) {
			GuildMemberListGroup g;
			rapidjson::Value *ptr = nullptr;

			if( (ptr = rapidjson::Pointer((pointer + "/id").c_str()).Get(doc)) && ptr->IsString())
				g.id = ptr->GetString();

			if( (ptr = rapidjson::Pointer((pointer + "/count").c_str()).Get(doc)) && ptr->IsInt())
				g.count = ptr->GetInt();

			return g;
		}
	};

	struct GuildMemberListUpdateOperation : Deserializable_Serializable_Class<GuildMemberListUpdateOperation> {
		std::pair<int32_t, int32_t> range;
		std::string op;
		std::vector< std::variant<GuildMemberListGroup, Member> > items;

		static GuildMemberListUpdateOperation LoadFrom(rapidjson::Document &doc, std::string pointer) {
			GuildMemberListUpdateOperation g;
			rapidjson::Value *ptr = nullptr;

			if( (ptr = rapidjson::Pointer((pointer + "/op").c_str()).Get(doc)) && ptr->IsString())
				g.op = ptr->GetString();
			
			if( (ptr = rapidjson::Pointer((pointer + "/range").c_str()).Get(doc)) && ptr->IsArray())
				g.range = std::make_pair<int32_t, int32_t>(
					rapidjson::Pointer((pointer + "/range/0").c_str()).Get(doc)->GetInt(),
					rapidjson::Pointer((pointer + "/range/1").c_str()).Get(doc)->GetInt()
				);

			if( (ptr = rapidjson::Pointer((pointer + "/items").c_str()).Get(doc)) && ptr->IsArray()) {
				int i = 0;
				for(rapidjson::Value& item : ptr->GetArray()) {
					if(item.IsObject()) {
						if(item.HasMember("member")) {
							g.items.push_back( Member::LoadFrom(doc, pointer + "/items/" + std::to_string(i) + "/member") );
						}else if(item.HasMember("group")) {
							g.items.push_back( GuildMemberListGroup::LoadFrom(doc, pointer + "/items/" + std::to_string(i) + "/group") );
						}

					}
					i++;
				}
			}

			return g;
		}
	};

	struct GuildMemberListUpdatePacket : Deserializable_Serializable_Class<GuildMemberListUpdatePacket> {
		std::vector<GuildMemberListUpdateOperation> operations;

		std::string id; // NOT a Snowflake
		Snowflake guildID;
		std::vector<GuildMemberListGroup> groups;

		static GuildMemberListUpdatePacket LoadFrom(rapidjson::Document &doc, std::string pointer) {
			GuildMemberListUpdatePacket g;
			rapidjson::Value *ptr = nullptr;

			if( (ptr = rapidjson::Pointer((pointer + "/id").c_str()).Get(doc)) && ptr->IsString())
				g.id = ptr->GetString();
			
			if( (ptr = rapidjson::Pointer((pointer + "/guild_id").c_str()).Get(doc)) && ptr->IsString())
				g.guildID = Snowflake(ptr->GetString());

			if( (ptr = rapidjson::Pointer((pointer + "/ops").c_str()).Get(doc)) && ptr->IsArray()) {
				int i=0;
				for(auto& element : ptr->GetArray())
					g.operations.push_back(GuildMemberListUpdateOperation::LoadFrom(doc, pointer + "/ops/" + std::to_string(i++)));
			}

			if( (ptr = rapidjson::Pointer((pointer + "/groups").c_str()).Get(doc)) && ptr->IsArray()) {
				for(uint32_t i=0; i<ptr->GetArray().Size(); i++)
					g.groups.push_back(GuildMemberListGroup::LoadFrom(doc, pointer + "/groups/" + std::to_string(i)));
			}

			return g;
		}
	};

	struct ReadyPacket : Deserializable_Serializable_Class<ReadyPacket> {
		int32_t version;
		User user;

		std::vector<Channel> privateChannels;
		std::vector<Guild> guilds;
		std::string sessionID;
		// Used for sending Discord's "science" requests.
		std::string analyticsToken; // note for future: This is one of the many things which do not exist for Bot accounts.

		static ReadyPacket LoadFrom(rapidjson::Document& doc, std::string pointer = "");

	};

	struct TypingStartPacket : Deserializable_Serializable_Class<TypingStartPacket> {
		Snowflake userID;
		std::optional<Snowflake> guildID;
		Snowflake channelID;

		std::optional<Member> member;

		uint64_t timestamp;

		static TypingStartPacket LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

	// Used for MESSAGE_REACTION_ADD and MESSAGE_REACTION_REMOVE events.
	// Not used for MESSAGE_REACTION_REMOVE_ALL.
	// https://discordapp.com/developers/docs/topics/gateway#message-reaction-add
	struct MessageReactionPacket : Deserializable_Serializable_Class<MessageReactionPacket> {
		Snowflake userID;
		Snowflake channelID;
		Snowflake messageID;
		std::optional<Snowflake> guildID;
		Emoji emoji;

		static MessageReactionPacket LoadFrom(rapidjson::Document& doc, std::string pointer = "");
	};

}