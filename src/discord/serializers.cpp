#include <discord/guild.hpp>
#include <discord/member.hpp>
#include <discord/user.hpp>
#include <discord/channel.hpp>
#include <discord/emoji.hpp>
#include <discord/message.hpp>

#include <discord/snowflake.hpp>

/*
	This file contains automatically generated static functions for
	parsing JSON Documents into this project's Discord Object
	representations.

	It is not pretty to read.
 */

// This function was generated by scripts/serializer_generator.py on Fri Jun 21 17:10:38 2019.
Discord::Guild Discord::Guild::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	Guild g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/id").c_str()).Get(doc)) && ptr->IsString())
		g.id = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/name").c_str()).Get(doc)) && ptr->IsString())
		g.name = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/splash_hash").c_str()).Get(doc)) && ptr->IsString())
		g.splashHash = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/icon_hash").c_str()).Get(doc)) && ptr->IsString())
		g.iconHash = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/owner_id").c_str()).Get(doc)) && ptr->IsString())
		g.ownerID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/permissions").c_str()).Get(doc)) && ptr->IsInt())
		g.permissions = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/region").c_str()).Get(doc)) && ptr->IsString())
		g.region = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/afk_channel_id").c_str()).Get(doc)) && ptr->IsString())
		g.afkChannelID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/afk_timeout").c_str()).Get(doc)) && ptr->IsInt())
		g.afkTimeout = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/embedable").c_str()).Get(doc)) && ptr->IsBool())
		g.embedable = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/embed_channel_id").c_str()).Get(doc)) && ptr->IsString())
		g.embedChannelID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/verification_level").c_str()).Get(doc)) && ptr->IsInt())
		g.verificationLevel = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/default_message_notifications_level").c_str()).Get(doc)) && ptr->IsInt())
		g.defaultMessageNotificationsLevel = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/explicit_content_filter_level").c_str()).Get(doc)) && ptr->IsInt())
		g.explicitContentFilterLevel = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/mfa_level").c_str()).Get(doc)) && ptr->IsInt())
		g.mfaLevel = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/roles").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.roles.push_back(Role::LoadFrom(doc, pointer + "/roles/" + std::to_string(i++)));
	}

	if( (ptr = rapidjson::Pointer((pointer + "/emoji").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.emoji.push_back(Emoji::LoadFrom(doc, pointer + "/emoji/" + std::to_string(i++)));
	}

	if( (ptr = rapidjson::Pointer((pointer + "/features").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			if(element.IsString())
				g.features.push_back(element.GetString());
	}

	if( (ptr = rapidjson::Pointer((pointer + "/application_id").c_str()).Get(doc)) && ptr->IsString())
		g.applicationID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/widget_enabled").c_str()).Get(doc)) && ptr->IsBool())
		g.widgetEnabled = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/widget_channel_id").c_str()).Get(doc)) && ptr->IsString())
		g.widgetChannelID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/system_channel_id").c_str()).Get(doc)) && ptr->IsString())
		g.systemChannelID = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/joined_at").c_str()).Get(doc)) && ptr->IsString())
		g.joinedAt = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/large").c_str()).Get(doc)) && ptr->IsBool())
		g.large = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/unavailable").c_str()).Get(doc)) && ptr->IsBool())
		g.unavailable = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/member_count").c_str()).Get(doc)) && ptr->IsInt())
		g.memberCount = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/members").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.members.push_back(Member::LoadFrom(doc, pointer + "/members/" + std::to_string(i++)));
	}

	if( (ptr = rapidjson::Pointer((pointer + "/channels").c_str()).Get(doc)) && ptr->IsArray()) {
		int i=0;
		for(auto& element : ptr->GetArray())
			g.channels.push_back(Channel::LoadFrom(doc, pointer + "/channels/" + std::to_string(i++)));
	}

	if( (ptr = rapidjson::Pointer((pointer + "/max_presences").c_str()).Get(doc)) && ptr->IsInt())
		g.maxPresences = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/max_members").c_str()).Get(doc)) && ptr->IsInt())
		g.maxMembers = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/premium_tier").c_str()).Get(doc)) && ptr->IsInt())
		g.premiumTier = ptr->GetInt();

	if( (ptr = rapidjson::Pointer((pointer + "/description").c_str()).Get(doc)) && ptr->IsString())
		g.description = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/banner_hash").c_str()).Get(doc)) && ptr->IsString())
		g.bannerHash = ptr->GetString();

	return g;
}

Discord::Emoji Discord::Emoji::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	Emoji g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/id").c_str()).Get(doc)) && ptr->IsString())
		g.id = Snowflake(ptr->GetString());

	if( (ptr = rapidjson::Pointer((pointer + "/name").c_str()).Get(doc)) && ptr->IsString())
		g.name = ptr->GetString();

	if( (ptr = rapidjson::Pointer((pointer + "/require_colons").c_str()).Get(doc)) && ptr->IsBool())
		g.requireColons = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/managed").c_str()).Get(doc)) && ptr->IsBool())
		g.managed = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/animated").c_str()).Get(doc)) && ptr->IsBool())
		g.animated = ptr->GetBool();

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

	return g;
}

// This function was generated by scripts/serializer_generator.py on Fri Jun 21 17:21:39 2019.
Discord::Member Discord::Member::LoadFrom(rapidjson::Document &doc, std::string pointer) {
	using namespace Discord;
	Member g;
	rapidjson::Value *ptr = nullptr;

	if( (ptr = rapidjson::Pointer((pointer + "/nickname").c_str()).Get(doc)) && ptr->IsString())
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

	if( (ptr = rapidjson::Pointer((pointer + "/pinned").c_str()).Get(doc)) && ptr->IsBool())
		g.pinned = ptr->GetBool();

	if( (ptr = rapidjson::Pointer((pointer + "/webhook_id").c_str()).Get(doc)) && ptr->IsString())
		g.webhookID = Snowflake(ptr->GetString());

	return g;
}