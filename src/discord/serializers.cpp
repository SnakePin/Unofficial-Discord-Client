#include <stdint.h>
#include <optional>
#include <vector>
#include <rapidjson/pointer.h>
#include <rapidjson/document.h>
#include "discord/guild.hpp"
#include "discord/member.hpp"
#include "discord/user.hpp"
#include "discord/channel.hpp"
#include "discord/emoji.hpp"
#include "discord/message.hpp"
#include "discord/packets.hpp"
#include "discord/snowflake.hpp"
#include "discord/attachment.hpp"

/*
	This file contains automatically generated static functions for
	parsing JSON Documents into this project's Discord Object
	representations.

	It is not pretty to read.
 */

#pragma region JsonTypeToStructType functions
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, std::string& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsString())
	{
		structFieldRef = ptr->GetString();
		return true;
	}
	return false;
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint32_t& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsUint())
	{
		structFieldRef = ptr->IsUint();
		return true;
	}
	return false;
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int32_t& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsInt())
	{
		structFieldRef = ptr->IsInt();
		return true;
	}
	return false;
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int64_t& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsInt64())
	{
		structFieldRef = ptr->GetInt64();
		return true;
	}
	return false;
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint64_t& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsUint64())
	{
		structFieldRef = ptr->GetUint64();
		return true;
	}
	return false;
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint8_t& structFieldRef)
{
	return JsonTypeToStructType(doc, pointer, (int32_t&)structFieldRef);
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int8_t& structFieldRef)
{
	return JsonTypeToStructType(doc, pointer, (int32_t&)structFieldRef);
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint16_t& structFieldRef)
{
	return JsonTypeToStructType(doc, pointer, (int32_t&)structFieldRef);
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int16_t& structFieldRef)
{
	return JsonTypeToStructType(doc, pointer, (int32_t&)structFieldRef);
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, bool& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsBool())
	{
		structFieldRef = ptr->GetBool();
		return true;
	}
	return false;
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, float& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsFloat())
	{
		structFieldRef = ptr->GetFloat();
		return true;
	}
	return false;
}
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, double& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsDouble())
	{
		structFieldRef = ptr->GetDouble();
		return true;
	}
	return false;
}

//Warning: This function has no error checking, it will return true even if json field was not found
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, Discord::Attachment& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	structFieldRef = Discord::Attachment::LoadFrom(doc, pointer);
	return true;
}

static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, Discord::Snowflake& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsUint64())
	{
		structFieldRef = Discord::Snowflake(ptr->GetUint64());
		return true;
	}
	return false;
}
template <typename T>
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, std::optional<T>& structFieldRef)
{
	T tempVar;
	if (JsonTypeToStructType(doc, pointer, tempVar)) {
		structFieldRef = tempVar;
		return true;
	}
	return false;
}
template <typename T>
static bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, std::vector<T>& structFieldRef)
{
	rapidjson::Value* ptr = nullptr;
	if ((ptr = rapidjson::Pointer(pointer.c_str()).Get(doc)) && ptr->IsArray()) {
		int i = 0;
		for (auto& element : ptr->GetArray()) {
			T tempVar;
			JsonTypeToStructType(doc, pointer + "/" + std::to_string(i++), tempVar);
			structFieldRef.push_back(tempVar);
		}
		return true;
	}
	return false;
}
#pragma endregion

Discord::Guild Discord::Guild::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	Guild g;
	rapidjson::Value* ptr = nullptr;

	JsonTypeToStructType(doc, pointer + "/id", g.id);
	JsonTypeToStructType(doc, pointer + "/name", g.name);
	JsonTypeToStructType(doc, pointer + "/splash_hash", g.splashHash);
	JsonTypeToStructType(doc, pointer + "/icon_hash", g.iconHash);
	JsonTypeToStructType(doc, pointer + "/owner_id", g.ownerID);
	JsonTypeToStructType(doc, pointer + "/permissions", g.permissions);
	JsonTypeToStructType(doc, pointer + "/region", g.region);
	JsonTypeToStructType(doc, pointer + "/afk_channel_id", g.afkChannelID);
	JsonTypeToStructType(doc, pointer + "/afk_timeout", g.afkTimeout);
	JsonTypeToStructType(doc, pointer + "/embedable", g.embedable);
	JsonTypeToStructType(doc, pointer + "/embed_channel_id", g.embedChannelID);
	JsonTypeToStructType(doc, pointer + "/verification_level", g.verificationLevel);
	JsonTypeToStructType(doc, pointer + "/default_message_notifications_level", g.defaultMessageNotificationsLevel);
	JsonTypeToStructType(doc, pointer + "/explicit_content_filter_level", g.explicitContentFilterLevel);
	JsonTypeToStructType(doc, pointer + "/mfa_level", g.mfaLevel);
	if ((ptr = rapidjson::Pointer((pointer + "/roles").c_str()).Get(doc)) && ptr->IsArray()) {
		int i = 0;
		for (auto& element : ptr->GetArray())
			g.roles.push_back(Role::LoadFrom(doc, pointer + "/roles/" + std::to_string(i++)));
	}
	if ((ptr = rapidjson::Pointer((pointer + "/emoji").c_str()).Get(doc)) && ptr->IsArray()) {
		int i = 0;
		for (auto& element : ptr->GetArray())
			g.emoji.push_back(Emoji::LoadFrom(doc, pointer + "/emoji/" + std::to_string(i++)));
	}
	if ((ptr = rapidjson::Pointer((pointer + "/features").c_str()).Get(doc)) && ptr->IsArray()) {
		for (auto& element : ptr->GetArray())
			if (element.IsString())
				g.features.push_back(element.GetString());
	}
	JsonTypeToStructType(doc, pointer + "/application_id", g.applicationID);
	JsonTypeToStructType(doc, pointer + "/widget_enabled", g.widgetEnabled);
	JsonTypeToStructType(doc, pointer + "/widget_channel_id", g.widgetChannelID);
	JsonTypeToStructType(doc, pointer + "/system_channel_id", g.systemChannelID);
	JsonTypeToStructType(doc, pointer + "/joined_at", g.joinedAt);
	JsonTypeToStructType(doc, pointer + "/large", g.large);
	JsonTypeToStructType(doc, pointer + "/unavailable", g.unavailable);
	JsonTypeToStructType(doc, pointer + "/member_count", g.memberCount);
	if ((ptr = rapidjson::Pointer((pointer + "/members").c_str()).Get(doc)) && ptr->IsArray()) {
		int i = 0;
		for (auto& element : ptr->GetArray())
			g.members.push_back(Member::LoadFrom(doc, pointer + "/members/" + std::to_string(i++)));
	}
	if ((ptr = rapidjson::Pointer((pointer + "/channels").c_str()).Get(doc)) && ptr->IsArray()) {
		int i = 0;
		for (auto& element : ptr->GetArray())
			g.channels.push_back(Channel::LoadFrom(doc, pointer + "/channels/" + std::to_string(i++)));
	}
	JsonTypeToStructType(doc, pointer + "/max_presences", g.maxPresences);
	JsonTypeToStructType(doc, pointer + "/max_members", g.maxMembers);
	JsonTypeToStructType(doc, pointer + "/premium_tier", g.premiumTier);
	JsonTypeToStructType(doc, pointer + "/description", g.description);
	JsonTypeToStructType(doc, pointer + "/banner_hash", g.bannerHash);

	return g;
}

Discord::Emoji Discord::Emoji::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	Emoji g;

	JsonTypeToStructType(doc, pointer + "/id", g.id);
	JsonTypeToStructType(doc, pointer + "/name", g.name);
	JsonTypeToStructType(doc, pointer + "/require_colons", g.requireColons);
	JsonTypeToStructType(doc, pointer + "/managed", g.managed);
	JsonTypeToStructType(doc, pointer + "/animated", g.animated);

	return g;
}

// This function was generated by scripts/serializer_generator.py on Fri Jun 21 17:20:35 2019.
Discord::Channel Discord::Channel::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	Channel g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/id").c_str()).Get(doc)) && ptr->IsString())
		g.id = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/type").c_str()).Get(doc)) && ptr->IsInt())
		g.type = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/name").c_str()).Get(doc)) && ptr->IsString())
		g.name = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/topic").c_str()).Get(doc)) && ptr->IsString())
		g.topic = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/nsfw").c_str()).Get(doc)) && ptr->IsBool())
		g.nsfw = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/icon_hash").c_str()).Get(doc)) && ptr->IsString())
		g.iconHash = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/guild_id").c_str()).Get(doc)) && ptr->IsString())
		g.guildID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/owner_id").c_str()).Get(doc)) && ptr->IsString())
		g.ownerID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/application_id").c_str()).Get(doc)) && ptr->IsString())
		g.applicationID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/parent_category_id").c_str()).Get(doc)) && ptr->IsString())
		g.parentCategoryID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/last_message_id").c_str()).Get(doc)) && ptr->IsString())
		g.lastMessageID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/position").c_str()).Get(doc)) && ptr->IsInt())
		g.position = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/permission_overwrites").c_str()).Get(doc)) && ptr->IsInt())
		g.permissionOverwrites = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/bitrate").c_str()).Get(doc)) && ptr->IsInt())
		g.bitrate = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/user_limit").c_str()).Get(doc)) && ptr->IsInt())
		g.userLimit = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/rate_limit_per_user").c_str()).Get(doc)) && ptr->IsInt())
		g.rateLimitPerUser = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/last_pin_timestamp").c_str()).Get(doc)) && ptr->IsString())
		g.lastPinTimestamp = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/recipients").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray()) {
			if(!g.recipients) g.recipients = std::vector<User>();
			g.recipients.value().push_back(User::LoadFrom(doc, pointer + "/recipients/" + std::to_string(i++)));
		}
	}

	return g;
}

// This function was generated by scripts/serializer_generator.py on Fri Jun 21 17:21:39 2019.
Discord::Member Discord::Member::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	Member g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/nick").c_str()).Get(doc)) && ptr->IsString())
		g.nickname = ptr->GetString();

	// This is an array of std::string (Snowflakes), NOT Discord::Role
	if( (ptr = rapidjson::Pointer((pointer + "/roles").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.roles.push_back( Snowflake(element.GetString()) );
	}

	if( (ptr = rapidjson::Pointer((pointer + "/joined_at").c_str()).Get(doc)) && ptr->IsString())
		g.joinedAt = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/premium_since").c_str()).Get(doc)) && ptr->IsString())
		g.premiumSince = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/deaf").c_str()).Get(doc)) && ptr->IsBool())
		g.deaf = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/mute").c_str()).Get(doc)) && ptr->IsBool())
		g.mute = ptr->GetBool();
	
	if( (ptr = rapidjson::Pointer((pointer + "/user").c_str()).Get(doc)) && ptr->IsObject())
		g.user = User::LoadFrom(doc, pointer + "/user");

	return g;
}

// This function was generated by scripts/serializer_generator.py on Fri Jun 21 17:22:30 2019.
Discord::User Discord::User::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	User g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/id").c_str()).Get(doc)) && ptr->IsString())
		g.id = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/username").c_str()).Get(doc)) && ptr->IsString())
		g.username = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/discriminator").c_str()).Get(doc)) && ptr->IsString())
		g.discriminator = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/avatar_hash").c_str()).Get(doc)) && ptr->IsString())
		g.avatarHash = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/bot").c_str()).Get(doc)) && ptr->IsBool())
		g.bot = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/mfa_enabled").c_str()).Get(doc)) && ptr->IsBool())
		g.mfaEnabled = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/locale").c_str()).Get(doc)) && ptr->IsString())
		g.locale = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/flags").c_str()).Get(doc)) && ptr->IsInt())
		g.flags = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/premium_type").c_str()).Get(doc)) && ptr->IsInt())
		g.premiumType = ptr->GetInt();

	return g;
}

// This function was generated by scripts/serializer_generator.py on Fri Jun 21 17:16:20 2019.
Discord::Role Discord::Role::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	Role g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/id").c_str()).Get(doc)) && ptr->IsString())
		g.id = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/name").c_str()).Get(doc)) && ptr->IsString())
		g.name = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/color").c_str()).Get(doc)) && ptr->IsInt())
		g.color = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/hoist").c_str()).Get(doc)) && ptr->IsBool())
		g.hoist = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/position").c_str()).Get(doc)) && ptr->IsInt())
		g.position = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/permissions").c_str()).Get(doc)) && ptr->IsInt())
		g.permissions = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/managed").c_str()).Get(doc)) && ptr->IsBool())
		g.managed = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/mentionable").c_str()).Get(doc)) && ptr->IsBool())
		g.mentionable = ptr->GetBool();

	return g;
}

// This function was generated by serializer_generator.py on Wed Jun 26 01:22:43 2019.
Discord::Message Discord::Message::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	Message g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/id").c_str()).Get(doc)) && ptr->IsString())
		g.id = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/channel_id").c_str()).Get(doc)) && ptr->IsString())
		g.channelID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/guild_id").c_str()).Get(doc)) && ptr->IsString())
		g.guildID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/nonce").c_str()).Get(doc)) && ptr->IsString())
		g.nonce = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/type").c_str()).Get(doc)) && ptr->IsInt())
		g.type = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/author").c_str()).Get(doc)) && ptr->IsObject())
		g.author = User::LoadFrom(doc, pointer + "/author");

	if( (ptr = rapidjson::Pointer((pointer + "/content").c_str()).Get(doc)) && ptr->IsString())
		g.content = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/timestamp").c_str()).Get(doc)) && ptr->IsString())
		g.timestamp = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/edited_timestamp").c_str()).Get(doc)) && ptr->IsString())
		g.editedTimestamp = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/tts").c_str()).Get(doc)) && ptr->IsBool())
		g.tts = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/mention_everyone").c_str()).Get(doc)) && ptr->IsBool())
		g.mentionEveryone = ptr->GetBool();

	/* TODO
	if( (ptr = rapidjson::Pointer((pointer + "/mentionedrole_ids").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.mentionedroleIDs.push_back(Role::LoadFrom(doc, pointer + "/mentionedrole_ids/" + std::to_string(i++)));
	}

	if( (ptr = rapidjson::Pointer((pointer + "/embeds").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.embeds.push_back(TARGETNAME::LoadFrom(doc, pointer + "/embeds/" + std::to_string(i++)));
	}

	if( (ptr = rapidjson::Pointer((pointer + "/reactions").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.reactions.push_back(Reaction::LoadFrom(doc, pointer + "/reactions/" + std::to_string(i++)));
	}*/

	JsonTypeToStructType(doc, pointer + "/attachments", g.attachments);

	if( (ptr = rapidjson::Pointer((pointer + "/pinned").c_str()).Get(doc)) && ptr->IsBool())
		g.pinned = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/webhook_id").c_str()).Get(doc)) && ptr->IsString())
		g.webhookID = Snowflake(ptr->GetString());

	return g;
}

Discord::Attachment Discord::Attachment::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	Discord::Attachment g;

	JsonTypeToStructType(doc, pointer + "/id", g.id);
	JsonTypeToStructType(doc, pointer + "/filename", g.filename);
	JsonTypeToStructType(doc, pointer + "/size", g.size);
	JsonTypeToStructType(doc, pointer + "/url", g.url);
	JsonTypeToStructType(doc, pointer + "/proxy_url", g.proxy_url);
	JsonTypeToStructType(doc, pointer + "/width", g.width);
	JsonTypeToStructType(doc, pointer + "/height", g.height);

	return g;
}

// This function was generated by scripts/serializer_generator.py on Thu Jun 27 00:15:36 2019.
Discord::ReadyPacket Discord::ReadyPacket::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	ReadyPacket g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/v").c_str()).Get(doc)) && ptr->IsInt())
		g.version = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/user").c_str()).Get(doc)) && ptr->IsObject())
		g.user = User::LoadFrom(doc, pointer + "/user");

	if( (ptr = rapidjson::Pointer((pointer + "/private_channels").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.privateChannels.push_back(Channel::LoadFrom(doc, pointer + "/private_channels/" + std::to_string(i++)));
	}

	if( (ptr = rapidjson::Pointer((pointer + "/guilds").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.guilds.push_back(Guild::LoadFrom(doc, pointer + "/guilds/" + std::to_string(i++)));
	}

	if( (ptr = rapidjson::Pointer((pointer + "/session_id").c_str()).Get(doc)) && ptr->IsString())
		g.sessionID = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/analytics_token").c_str()).Get(doc)) && ptr->IsString())
		g.analyticsToken = ptr->GetString();
	
	return g;
}

// This function was generated by scripts/serializer_generator.py on Thu Jun 27 01:43:14 2019.
Discord::TypingStartPacket Discord::TypingStartPacket::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	TypingStartPacket g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/user_id").c_str()).Get(doc)) && ptr->IsString())
		g.userID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/guild_id").c_str()).Get(doc)) && ptr->IsString())
		g.guildID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/channel_id").c_str()).Get(doc)) && ptr->IsString())
		g.channelID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/member").c_str()).Get(doc)) && ptr->IsObject())
		g.member = Member::LoadFrom(doc, pointer + "/member");

	if( (ptr = rapidjson::Pointer((pointer + "/timestamp").c_str()).Get(doc)) && ptr->IsInt())
		g.timestamp = ptr->GetInt();

	return g;
}

Discord::MessageReactionPacket Discord::MessageReactionPacket::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	MessageReactionPacket g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/user_id").c_str()).Get(doc)) && ptr->IsString())
		g.userID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/guild_id").c_str()).Get(doc)) && ptr->IsString())
		g.guildID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/channel_id").c_str()).Get(doc)) && ptr->IsString())
		g.channelID = Snowflake(ptr->GetString());
	
	if( (ptr = rapidjson::Pointer((pointer + "/message_id").c_str()).Get(doc)) && ptr->IsString())
		g.messageID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/emoji").c_str()).Get(doc)) && ptr->IsObject())
		g.emoji = Emoji::LoadFrom(doc, pointer + "/emoji");

	return g;
}