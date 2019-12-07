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
#include "discord/deserializable_serializable_class_type.hpp"
#include "discord/deserializers.hpp"

/*
	This file contains functions for
	parsing JSON Documents into this project's Discord Object
	representations.

	It is pretty to read except for the JsonTypeToStructType functions region.
 */

Discord::Guild Discord::Guild::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	Guild g;

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
	JsonTypeToStructType(doc, pointer + "/roles", g.roles);
	JsonTypeToStructType(doc, pointer + "/emoji", g.emoji);
	JsonTypeToStructType(doc, pointer + "/features", g.features);
	JsonTypeToStructType(doc, pointer + "/application_id", g.applicationID);
	JsonTypeToStructType(doc, pointer + "/widget_enabled", g.widgetEnabled);
	JsonTypeToStructType(doc, pointer + "/widget_channel_id", g.widgetChannelID);
	JsonTypeToStructType(doc, pointer + "/system_channel_id", g.systemChannelID);
	JsonTypeToStructType(doc, pointer + "/joined_at", g.joinedAt);
	JsonTypeToStructType(doc, pointer + "/large", g.large);
	JsonTypeToStructType(doc, pointer + "/unavailable", g.unavailable);
	JsonTypeToStructType(doc, pointer + "/member_count", g.memberCount);
	JsonTypeToStructType(doc, pointer + "/members", g.members);
	JsonTypeToStructType(doc, pointer + "/channels", g.channels);
	JsonTypeToStructType(doc, pointer + "/max_presences", g.maxPresences);
	JsonTypeToStructType(doc, pointer + "/max_members", g.maxMembers);
	JsonTypeToStructType(doc, pointer + "/premium_tier", g.premiumTier);
	JsonTypeToStructType(doc, pointer + "/description", g.description);
	JsonTypeToStructType(doc, pointer + "/banner_hash", g.bannerHash);

	return g;
}

Discord::Emoji Discord::Emoji::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	Emoji g;

	JsonTypeToStructType(doc, pointer + "/id", g.id);
	JsonTypeToStructType(doc, pointer + "/name", g.name);
	JsonTypeToStructType(doc, pointer + "/require_colons", g.requireColons);
	JsonTypeToStructType(doc, pointer + "/managed", g.managed);
	JsonTypeToStructType(doc, pointer + "/animated", g.animated);

	return g;
}

Discord::Channel Discord::Channel::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	Channel g;

	JsonTypeToStructType(doc, pointer + "/id", g.id);
	JsonTypeToStructType(doc, pointer + "/type", g.type);
	JsonTypeToStructType(doc, pointer + "/name", g.name);
	JsonTypeToStructType(doc, pointer + "/topic", g.topic);
	JsonTypeToStructType(doc, pointer + "/nsfw", g.nsfw);
	JsonTypeToStructType(doc, pointer + "/icon_hash", g.iconHash);
	JsonTypeToStructType(doc, pointer + "/guild_id", g.guildID);
	JsonTypeToStructType(doc, pointer + "/owner_id", g.ownerID);
	JsonTypeToStructType(doc, pointer + "/application_id", g.applicationID);
	JsonTypeToStructType(doc, pointer + "/parent_category_id", g.parentCategoryID);
	JsonTypeToStructType(doc, pointer + "/last_message_id", g.lastMessageID);
	JsonTypeToStructType(doc, pointer + "/position", g.position);
	JsonTypeToStructType(doc, pointer + "/permission_overwrites", g.permissionOverwrites);
	JsonTypeToStructType(doc, pointer + "/user_limit", g.userLimit);
	JsonTypeToStructType(doc, pointer + "/rate_limit_per_user", g.rateLimitPerUser);
	JsonTypeToStructType(doc, pointer + "/last_pin_timestamp", g.lastPinTimestamp);
	JsonTypeToStructType(doc, pointer + "/recipients", g.recipients);

	return g;
}

Discord::Member Discord::Member::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	Member g;

	JsonTypeToStructType(doc, pointer + "/nick", g.nickname);
	JsonTypeToStructType(doc, pointer + "/roles", g.roles); // This is an array of std::string (Snowflakes), NOT Discord::Role
	JsonTypeToStructType(doc, pointer + "/joined_at", g.joinedAt);
	JsonTypeToStructType(doc, pointer + "/premium_since", g.premiumSince);
	JsonTypeToStructType(doc, pointer + "/deaf", g.deaf);
	JsonTypeToStructType(doc, pointer + "/mute", g.mute);
	JsonTypeToStructType(doc, pointer + "/user", g.user);

	return g;
}

Discord::UserFlags Discord::UserFlags::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	UserFlags g;

	JsonTypeToStructType(doc, pointer, g.flags);

	return g;
}

Discord::User Discord::User::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	User g;

	JsonTypeToStructType(doc, pointer + "/id", g.id);
	JsonTypeToStructType(doc, pointer + "/username", g.username);
	JsonTypeToStructType(doc, pointer + "/discriminator", g.discriminator);
	JsonTypeToStructType(doc, pointer + "/avatar_hash", g.avatarHash);
	JsonTypeToStructType(doc, pointer + "/bot", g.bot);
	JsonTypeToStructType(doc, pointer + "/mfa_enabled", g.mfaEnabled);
	JsonTypeToStructType(doc, pointer + "/locale", g.locale);
	JsonTypeToStructType(doc, pointer + "/email", g.email);
	JsonTypeToStructType(doc, pointer + "/verified", g.verified);
	JsonTypeToStructType(doc, pointer + "/flags", g.flags);
	JsonTypeToStructType(doc, pointer + "/premium_type", g.premiumType);

	return g;
}

Discord::Role Discord::Role::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	Role g;

	JsonTypeToStructType(doc, pointer + "/id", g.id);
	JsonTypeToStructType(doc, pointer + "/name", g.name);
	JsonTypeToStructType(doc, pointer + "/color", g.color);
	JsonTypeToStructType(doc, pointer + "/hoist", g.hoist);
	JsonTypeToStructType(doc, pointer + "/position", g.position);
	JsonTypeToStructType(doc, pointer + "/permissions", g.permissions);
	JsonTypeToStructType(doc, pointer + "/managed", g.managed);
	JsonTypeToStructType(doc, pointer + "/mentionable", g.mentionable);

	return g;
}

Discord::Message Discord::Message::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	Message g;

	JsonTypeToStructType(doc, pointer + "/id", g.id);
	JsonTypeToStructType(doc, pointer + "/channel_id", g.channelID);
	JsonTypeToStructType(doc, pointer + "/guild_id", g.guildID);
	JsonTypeToStructType(doc, pointer + "/nonce", g.nonce);
	JsonTypeToStructType(doc, pointer + "/type", g.type);
	JsonTypeToStructType(doc, pointer + "/author", g.author);
	JsonTypeToStructType(doc, pointer + "/content", g.content);
	JsonTypeToStructType(doc, pointer + "/timestamp", g.timestamp);
	JsonTypeToStructType(doc, pointer + "/edited_timestamp", g.editedTimestamp);
	JsonTypeToStructType(doc, pointer + "/tts", g.tts);
	JsonTypeToStructType(doc, pointer + "/mention_everyone", g.mentionEveryone);

	/* TODO
	if( (ptr = rapidjson::Pointer((pointer + "/embeds").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.embeds.push_back(TARGETNAME::LoadFrom(doc, pointer + "/embeds/" + std::to_string(i++)));
	}
	}*/

	JsonTypeToStructType(doc, pointer + "/mentionedrole_ids", g.mentionedroleIDs);
	JsonTypeToStructType(doc, pointer + "/reactions", g.reactions);
	JsonTypeToStructType(doc, pointer + "/attachments", g.attachments);
	JsonTypeToStructType(doc, pointer + "/pinned", g.pinned);
	JsonTypeToStructType(doc, pointer + "/webhook_id", g.webhookID);

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

Discord::Reaction Discord::Reaction::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	Discord::Reaction g;

	JsonTypeToStructType(doc, pointer + "/count", g.count);
	JsonTypeToStructType(doc, pointer + "/me", g.me);
	JsonTypeToStructType(doc, pointer + "/emoji", g.emoji);

	return g;
}

Discord::ReadyPacket Discord::ReadyPacket::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	ReadyPacket g;

	JsonTypeToStructType(doc, pointer + "/v", g.version);
	JsonTypeToStructType(doc, pointer + "/user", g.user);
	JsonTypeToStructType(doc, pointer + "/private_channels", g.privateChannels);
	JsonTypeToStructType(doc, pointer + "/guilds", g.guilds);
	JsonTypeToStructType(doc, pointer + "/session_id", g.sessionID);
	JsonTypeToStructType(doc, pointer + "/analytics_token", g.analyticsToken);

	return g;
}

Discord::TypingStartPacket Discord::TypingStartPacket::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	TypingStartPacket g;

	JsonTypeToStructType(doc, pointer + "/user_id", g.userID);
	JsonTypeToStructType(doc, pointer + "/guild_id", g.guildID);
	JsonTypeToStructType(doc, pointer + "/channel_id", g.channelID);
	JsonTypeToStructType(doc, pointer + "/member", g.member);
	JsonTypeToStructType(doc, pointer + "/timestamp", g.timestamp);

	return g;
}

Discord::MessageReactionPacket Discord::MessageReactionPacket::LoadFrom(rapidjson::Document& doc, std::string pointer) {
	MessageReactionPacket g;

	JsonTypeToStructType(doc, pointer + "/user_id", g.userID);
	JsonTypeToStructType(doc, pointer + "/guild_id", g.guildID);
	JsonTypeToStructType(doc, pointer + "/channel_id", g.channelID);
	JsonTypeToStructType(doc, pointer + "/message_id", g.messageID);
	JsonTypeToStructType(doc, pointer + "/emoji", g.emoji);

	return g;
}

#pragma region JsonTypeToStructType functions
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, std::string& structFieldRef)
{
	rapidjson::Value* ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsString())
	{
		structFieldRef = ptr->GetString();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint32_t& structFieldRef)
{
	rapidjson::Value* ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsUint())
	{
		structFieldRef = ptr->IsUint();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int32_t& structFieldRef)
{
	rapidjson::Value* ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsInt())
	{
		structFieldRef = ptr->GetInt();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int64_t& structFieldRef)
{
	rapidjson::Value* ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsInt64())
	{
		structFieldRef = ptr->GetInt64();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint64_t& structFieldRef)
{
	rapidjson::Value* ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsUint64())
	{
		structFieldRef = ptr->GetUint64();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint8_t& structFieldRef)
{
	int32_t tempVar;
	bool retVal = JsonTypeToStructType(doc, pointer, tempVar);
	structFieldRef = (uint8_t)tempVar;
	return retVal;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int8_t& structFieldRef)
{
	int32_t tempVar;
	bool retVal = JsonTypeToStructType(doc, pointer, tempVar);
	structFieldRef = (int8_t)tempVar;
	return retVal;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, uint16_t& structFieldRef)
{
	int32_t tempVar;
	bool retVal = JsonTypeToStructType(doc, pointer, tempVar);
	structFieldRef = (uint16_t)tempVar;
	return retVal;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, int16_t& structFieldRef)
{
	int32_t tempVar;
	bool retVal = JsonTypeToStructType(doc, pointer, tempVar);
	structFieldRef = (int16_t)tempVar;
	return retVal;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, bool& structFieldRef)
{
	rapidjson::Value* ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsBool())
	{
		structFieldRef = ptr->GetBool();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, float& structFieldRef)
{
	rapidjson::Value* ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsFloat())
	{
		structFieldRef = ptr->GetFloat();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, double& structFieldRef)
{
	rapidjson::Value* ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsDouble())
	{
		structFieldRef = ptr->GetDouble();
		return true;
	}
	return false;
}

//Warning: This function has no error checking, it will return true even if json field was not found
//This function will get called if structFieldRef is a reference to a class that implements Discord::Deserializable_Serializable_Class<T>.
//The template will be deduced from the derived class.
template <typename T>
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, Discord::Deserializable_Serializable_Class<T>& structFieldRef)
{
	//Boo, hacky hack below
	static_cast<T&>(structFieldRef) = T::LoadFrom(doc, pointer);
	return true;
}
#include "discord/utils.hpp"
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, Discord::Snowflake& structFieldRef)
{
	uint64_t tempVar = 0;
	if (JsonTypeToStructType(doc, pointer, tempVar)) {
		structFieldRef = Discord::Snowflake(tempVar);
		return true;
	}

	std::string tempStr;
	if (JsonTypeToStructType(doc, pointer, tempStr)) {
		structFieldRef = Discord::Snowflake(tempStr);
		return true;
	}

	return false;
}
template <typename T>
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, std::optional<T>& structFieldRef)
{
	T tempVar;
	if (JsonTypeToStructType(doc, pointer, tempVar)) {
		structFieldRef = tempVar;
		return true;
	}
	return false;
}
template <typename T>
bool JsonTypeToStructType(rapidjson::Document& doc, std::string pointer, std::vector<T>& structFieldRef)
{
	rapidjson::Value* ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsArray())
	{
		rapidjson::SizeType arraySize = ptr->GetArray().Size();
		for (rapidjson::SizeType i = 0; i < arraySize; i++) {
			T tempVar;
			JsonTypeToStructType(doc, pointer + "/" + std::to_string(i++), tempVar);
			structFieldRef.push_back(tempVar);
		}
		return true;
	}
	return false;
}
#pragma endregion