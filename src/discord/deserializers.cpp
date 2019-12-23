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
#include "discord/utils.hpp"

/*
	This file contains functions for
	parsing JSON Documents into this project's Discord Object
	representations.

	It is pretty to read except for the JsonTypeToStructType functions region.
 */
bool Discord::Guild::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/id", this->id);
	retVal &= JsonTypeToStructType(doc, pointer + "/name", this->name);
	retVal &= JsonTypeToStructType(doc, pointer + "/splash_hash", this->splashHash);
	retVal &= JsonTypeToStructType(doc, pointer + "/icon_hash", this->iconHash);
	retVal &= JsonTypeToStructType(doc, pointer + "/owner_id", this->ownerID);
	retVal &= JsonTypeToStructType(doc, pointer + "/permissions", this->permissions);
	retVal &= JsonTypeToStructType(doc, pointer + "/region", this->region);
	retVal &= JsonTypeToStructType(doc, pointer + "/afk_channel_id", this->afkChannelID);
	retVal &= JsonTypeToStructType(doc, pointer + "/afk_timeout", this->afkTimeout);
	retVal &= JsonTypeToStructType(doc, pointer + "/embedable", this->embedable);
	retVal &= JsonTypeToStructType(doc, pointer + "/embed_channel_id", this->embedChannelID);
	retVal &= JsonTypeToStructType(doc, pointer + "/verification_level", this->verificationLevel);
	retVal &= JsonTypeToStructType(doc, pointer + "/default_message_notifications_level", this->defaultMessageNotificationsLevel);
	retVal &= JsonTypeToStructType(doc, pointer + "/explicit_content_filter_level", this->explicitContentFilterLevel);
	retVal &= JsonTypeToStructType(doc, pointer + "/mfa_level", this->mfaLevel);
	retVal &= JsonTypeToStructType(doc, pointer + "/roles", this->roles);
	retVal &= JsonTypeToStructType(doc, pointer + "/emoji", this->emoji);
	retVal &= JsonTypeToStructType(doc, pointer + "/features", this->features);
	retVal &= JsonTypeToStructType(doc, pointer + "/application_id", this->applicationID);
	retVal &= JsonTypeToStructType(doc, pointer + "/widget_enabled", this->widgetEnabled);
	retVal &= JsonTypeToStructType(doc, pointer + "/widget_channel_id", this->widgetChannelID);
	retVal &= JsonTypeToStructType(doc, pointer + "/system_channel_id", this->systemChannelID);
	retVal &= JsonTypeToStructType(doc, pointer + "/joined_at", this->joinedAt);
	retVal &= JsonTypeToStructType(doc, pointer + "/large", this->large);
	retVal &= JsonTypeToStructType(doc, pointer + "/unavailable", this->unavailable);
	retVal &= JsonTypeToStructType(doc, pointer + "/member_count", this->memberCount);
	retVal &= JsonTypeToStructType(doc, pointer + "/members", this->members);
	retVal &= JsonTypeToStructType(doc, pointer + "/channels", this->channels);
	retVal &= JsonTypeToStructType(doc, pointer + "/max_presences", this->maxPresences);
	retVal &= JsonTypeToStructType(doc, pointer + "/max_members", this->maxMembers);
	retVal &= JsonTypeToStructType(doc, pointer + "/premium_tier", this->premiumTier);
	retVal &= JsonTypeToStructType(doc, pointer + "/description", this->description);
	retVal &= JsonTypeToStructType(doc, pointer + "/banner_hash", this->bannerHash);

	return retVal;
}

bool Discord::Emoji::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/id", this->id);
	retVal &= JsonTypeToStructType(doc, pointer + "/name", this->name);
	retVal &= JsonTypeToStructType(doc, pointer + "/require_colons", this->requireColons);
	retVal &= JsonTypeToStructType(doc, pointer + "/managed", this->managed);
	retVal &= JsonTypeToStructType(doc, pointer + "/animated", this->animated);

	return retVal;
}

bool Discord::Channel::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/id", this->id);
	retVal &= JsonTypeToStructType(doc, pointer + "/type", this->type);
	retVal &= JsonTypeToStructType(doc, pointer + "/name", this->name);
	retVal &= JsonTypeToStructType(doc, pointer + "/topic", this->topic);
	retVal &= JsonTypeToStructType(doc, pointer + "/nsfw", this->nsfw);
	retVal &= JsonTypeToStructType(doc, pointer + "/icon_hash", this->iconHash);
	retVal &= JsonTypeToStructType(doc, pointer + "/guild_id", this->guildID);
	retVal &= JsonTypeToStructType(doc, pointer + "/owner_id", this->ownerID);
	retVal &= JsonTypeToStructType(doc, pointer + "/application_id", this->applicationID);
	retVal &= JsonTypeToStructType(doc, pointer + "/parent_category_id", this->parentCategoryID);
	retVal &= JsonTypeToStructType(doc, pointer + "/last_message_id", this->lastMessageID);
	retVal &= JsonTypeToStructType(doc, pointer + "/position", this->position);
	retVal &= JsonTypeToStructType(doc, pointer + "/permission_overwrites", this->permissionOverwrites);
	retVal &= JsonTypeToStructType(doc, pointer + "/user_limit", this->userLimit);
	retVal &= JsonTypeToStructType(doc, pointer + "/rate_limit_per_user", this->rateLimitPerUser);
	retVal &= JsonTypeToStructType(doc, pointer + "/last_pin_timestamp", this->lastPinTimestamp);
	retVal &= JsonTypeToStructType(doc, pointer + "/recipients", this->recipients);

	return retVal;
}

bool Discord::Member::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/nick", this->nickname);
	retVal &= JsonTypeToStructType(doc, pointer + "/roles", this->roles); // This is an array of std::string (Snowflakes), NOT Discord::Role
	retVal &= JsonTypeToStructType(doc, pointer + "/joined_at", this->joinedAt);
	retVal &= JsonTypeToStructType(doc, pointer + "/premium_since", this->premiumSince);
	retVal &= JsonTypeToStructType(doc, pointer + "/deaf", this->deaf);
	retVal &= JsonTypeToStructType(doc, pointer + "/mute", this->mute);
	retVal &= JsonTypeToStructType(doc, pointer + "/user", this->user);

	return retVal;
}

bool Discord::UserFlags::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer, this->flags);

	return retVal;
}

bool Discord::User::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/id", this->id);
	retVal &= JsonTypeToStructType(doc, pointer + "/username", this->username);
	retVal &= JsonTypeToStructType(doc, pointer + "/discriminator", this->discriminator);
	retVal &= JsonTypeToStructType(doc, pointer + "/avatar_hash", this->avatarHash);
	retVal &= JsonTypeToStructType(doc, pointer + "/bot", this->bot);
	retVal &= JsonTypeToStructType(doc, pointer + "/mfa_enabled", this->mfaEnabled);
	retVal &= JsonTypeToStructType(doc, pointer + "/locale", this->locale);
	retVal &= JsonTypeToStructType(doc, pointer + "/email", this->email);
	retVal &= JsonTypeToStructType(doc, pointer + "/verified", this->verified);
	retVal &= JsonTypeToStructType(doc, pointer + "/flags", this->flags);
	retVal &= JsonTypeToStructType(doc, pointer + "/premium_type", this->premiumType);

	return retVal;
}

bool Discord::Role::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/id", this->id);
	retVal &= JsonTypeToStructType(doc, pointer + "/name", this->name);
	retVal &= JsonTypeToStructType(doc, pointer + "/color", this->color);
	retVal &= JsonTypeToStructType(doc, pointer + "/hoist", this->hoist);
	retVal &= JsonTypeToStructType(doc, pointer + "/position", this->position);
	retVal &= JsonTypeToStructType(doc, pointer + "/permissions", this->permissions);
	retVal &= JsonTypeToStructType(doc, pointer + "/managed", this->managed);
	retVal &= JsonTypeToStructType(doc, pointer + "/mentionable", this->mentionable);

	return retVal;
}

bool Discord::Message::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/id", this->id);
	retVal &= JsonTypeToStructType(doc, pointer + "/channel_id", this->channelID);
	retVal &= JsonTypeToStructType(doc, pointer + "/guild_id", this->guildID);
	retVal &= JsonTypeToStructType(doc, pointer + "/nonce", this->nonce);
	retVal &= JsonTypeToStructType(doc, pointer + "/type", this->type);
	retVal &= JsonTypeToStructType(doc, pointer + "/author", this->author);
	retVal &= JsonTypeToStructType(doc, pointer + "/content", this->content);
	retVal &= JsonTypeToStructType(doc, pointer + "/timestamp", this->timestamp);
	retVal &= JsonTypeToStructType(doc, pointer + "/edited_timestamp", this->editedTimestamp);
	retVal &= JsonTypeToStructType(doc, pointer + "/tts", this->tts);
	retVal &= JsonTypeToStructType(doc, pointer + "/mention_everyone", this->mentionEveryone);
	//TODO: retVal &= JsonTypeToStructType(doc, pointer + "/embeds", this->embeds);
	retVal &= JsonTypeToStructType(doc, pointer + "/mentionedrole_ids", this->mentionedroleIDs);
	retVal &= JsonTypeToStructType(doc, pointer + "/reactions", this->reactions);
	retVal &= JsonTypeToStructType(doc, pointer + "/attachments", this->attachments);
	retVal &= JsonTypeToStructType(doc, pointer + "/pinned", this->pinned);
	retVal &= JsonTypeToStructType(doc, pointer + "/webhook_id", this->webhookID);

	return retVal;
}

bool Discord::Attachment::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/id", this->id);
	retVal &= JsonTypeToStructType(doc, pointer + "/filename", this->filename);
	retVal &= JsonTypeToStructType(doc, pointer + "/size", this->size);
	retVal &= JsonTypeToStructType(doc, pointer + "/url", this->url);
	retVal &= JsonTypeToStructType(doc, pointer + "/proxy_url", this->proxy_url);
	retVal &= JsonTypeToStructType(doc, pointer + "/width", this->width);
	retVal &= JsonTypeToStructType(doc, pointer + "/height", this->height);

	return retVal;
}

bool Discord::Reaction::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/count", this->count);
	retVal &= JsonTypeToStructType(doc, pointer + "/me", this->me);
	retVal &= JsonTypeToStructType(doc, pointer + "/emoji", this->emoji);

	return retVal;
}

bool Discord::ReadyPacket::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/v", this->version);
	retVal &= JsonTypeToStructType(doc, pointer + "/user", this->user);
	retVal &= JsonTypeToStructType(doc, pointer + "/private_channels", this->privateChannels);
	retVal &= JsonTypeToStructType(doc, pointer + "/guilds", this->guilds);
	retVal &= JsonTypeToStructType(doc, pointer + "/session_id", this->sessionID);
	retVal &= JsonTypeToStructType(doc, pointer + "/analytics_token", this->analyticsToken);

	return retVal;
}

bool Discord::TypingStartPacket::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/user_id", this->userID);
	retVal &= JsonTypeToStructType(doc, pointer + "/guild_id", this->guildID);
	retVal &= JsonTypeToStructType(doc, pointer + "/channel_id", this->channelID);
	retVal &= JsonTypeToStructType(doc, pointer + "/member", this->member);
	retVal &= JsonTypeToStructType(doc, pointer + "/timestamp", this->timestamp);

	return retVal;
}

bool Discord::MessageReactionPacket::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/user_id", this->userID);
	retVal &= JsonTypeToStructType(doc, pointer + "/guild_id", this->guildID);
	retVal &= JsonTypeToStructType(doc, pointer + "/channel_id", this->channelID);
	retVal &= JsonTypeToStructType(doc, pointer + "/message_id", this->messageID);
	retVal &= JsonTypeToStructType(doc, pointer + "/emoji", this->emoji);

	return retVal;
}

bool Discord::GuildMemberListUpdatePacket::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/id", this->id);
	retVal &= JsonTypeToStructType(doc, pointer + "/guild_id", this->guildID);
	retVal &= JsonTypeToStructType(doc, pointer + "/ops", this->operations);
	retVal &= JsonTypeToStructType(doc, pointer + "/groups", this->groups);

	return retVal;
}

bool Discord::GuildMemberListUpdateOperation::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/op", this->op);

	rapidjson::Value *ptr = nullptr;
	if ((ptr = rapidjson::Pointer((pointer + "/range").c_str()).Get(doc)) && ptr->IsArray())
		this->range = std::make_pair<int32_t, int32_t>(
			rapidjson::Pointer((pointer + "/range/0").c_str()).Get(doc)->GetInt(),
			rapidjson::Pointer((pointer + "/range/1").c_str()).Get(doc)->GetInt());

	if ((ptr = rapidjson::Pointer((pointer + "/items").c_str()).Get(doc)) && ptr->IsArray())
	{
		int i = 0;
		for (rapidjson::Value &item : ptr->GetArray())
		{
			if (item.IsObject())
			{
				if (item.HasMember("member"))
				{
					Member tmpVar;
					tmpVar.LoadFrom(doc, pointer + "/items/" + std::to_string(i) + "/member");
					this->items.push_back(tmpVar);
				}
				else if (item.HasMember("group"))
				{
					GuildMemberListGroup tmpVar;
					tmpVar.LoadFrom(doc, pointer + "/items/" + std::to_string(i) + "/group");
					this->items.push_back(tmpVar);
				}
			}
			i++;
		}
	}

	return retVal;
}

bool Discord::GuildMemberListGroup::LoadFrom(rapidjson::Document &doc, std::string pointer)
{
	bool retVal = false;

	retVal &= JsonTypeToStructType(doc, pointer + "/id", this->id);
	retVal &= JsonTypeToStructType(doc, pointer + "/count", this->count);

	return retVal;
}

#pragma region JsonTypeToStructType functions
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, std::string &structFieldRef)
{
	rapidjson::Value *ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsString())
	{
		structFieldRef = ptr->GetString();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, uint32_t &structFieldRef)
{
	rapidjson::Value *ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsUint())
	{
		structFieldRef = ptr->IsUint();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, int32_t &structFieldRef)
{
	rapidjson::Value *ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsInt())
	{
		structFieldRef = ptr->GetInt();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, int64_t &structFieldRef)
{
	rapidjson::Value *ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsInt64())
	{
		structFieldRef = ptr->GetInt64();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, uint64_t &structFieldRef)
{
	rapidjson::Value *ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsUint64())
	{
		structFieldRef = ptr->GetUint64();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, uint8_t &structFieldRef)
{
	int32_t tempVar;
	bool retVal = JsonTypeToStructType(doc, pointer, tempVar);
	structFieldRef = (uint8_t)tempVar;
	return retVal;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, int8_t &structFieldRef)
{
	int32_t tempVar;
	bool retVal = JsonTypeToStructType(doc, pointer, tempVar);
	structFieldRef = (int8_t)tempVar;
	return retVal;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, uint16_t &structFieldRef)
{
	int32_t tempVar;
	bool retVal = JsonTypeToStructType(doc, pointer, tempVar);
	structFieldRef = (uint16_t)tempVar;
	return retVal;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, int16_t &structFieldRef)
{
	int32_t tempVar;
	bool retVal = JsonTypeToStructType(doc, pointer, tempVar);
	structFieldRef = (int16_t)tempVar;
	return retVal;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, bool &structFieldRef)
{
	rapidjson::Value *ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsBool())
	{
		structFieldRef = ptr->GetBool();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, float &structFieldRef)
{
	rapidjson::Value *ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsFloat())
	{
		structFieldRef = ptr->GetFloat();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, double &structFieldRef)
{
	rapidjson::Value *ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsDouble())
	{
		structFieldRef = ptr->GetDouble();
		return true;
	}
	return false;
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, Discord::JsonDeSerializable &structFieldRef)
{
	return structFieldRef.LoadFrom(doc, pointer);
}
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, Discord::Snowflake &structFieldRef)
{
	uint64_t tempVar = 0;
	if (JsonTypeToStructType(doc, pointer, tempVar))
	{
		structFieldRef = Discord::Snowflake(tempVar);
		return true;
	}

	std::string tempStr;
	if (JsonTypeToStructType(doc, pointer, tempStr))
	{
		structFieldRef = Discord::Snowflake(tempStr);
		return true;
	}

	return false;
}
template <typename T>
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, std::optional<T> &structFieldRef)
{
	T tempVar;
	if (JsonTypeToStructType(doc, pointer, tempVar))
	{
		structFieldRef = tempVar;
		return true;
	}
	return false;
}
template <typename T>
bool JsonTypeToStructType(rapidjson::Document &doc, std::string pointer, std::vector<T> &structFieldRef)
{
	rapidjson::Value *ptr = rapidjson::Pointer(pointer.c_str()).Get(doc);
	if (ptr != nullptr && ptr->IsArray())
	{
		rapidjson::SizeType arraySize = ptr->GetArray().Size();
		for (rapidjson::SizeType i = 0; i < arraySize; i++)
		{
			T tempVar;
			JsonTypeToStructType(doc, pointer + "/" + std::to_string(i++), tempVar);
			structFieldRef.push_back(tempVar);
		}
		return true;
	}
	return false;
}
#pragma endregion