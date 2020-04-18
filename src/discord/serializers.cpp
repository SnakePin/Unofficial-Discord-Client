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
#include "discord/jsonDeSerializable_class.hpp"
#include "discord/serializers.hpp"
#include "discord/InternalUtils.hpp"

/*
	This file contains functions for
	parsing JSON Documents into this project's Discord Object
	representations.

	It is pretty to read except for the JsonTypeToStructType functions region.
 */

//This macro is for making this file easier to read, it is only to be used in this file
#define JTTST_EZ(jsonObject, memberName) retVal &= JsonTypeToStructType(jsonObject, memberName)

using namespace Discord;

bool Guild::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;
	
	JTTST_EZ(object["id"], this->id);
	JTTST_EZ(object["name"], this->name);
	JTTST_EZ(object["splash_hash"], this->splashHash);
	JTTST_EZ(object["icon_hash"], this->iconHash);
	JTTST_EZ(object["owner_id"], this->ownerID);
	JTTST_EZ(object["permissions"], this->permissions);
	JTTST_EZ(object["region"], this->region);
	JTTST_EZ(object["afk_channel_id"], this->afkChannelID);
	JTTST_EZ(object["afk_timeout"], this->afkTimeout);
	JTTST_EZ(object["embedable"], this->embedable);
	JTTST_EZ(object["embed_channel_id"], this->embedChannelID);
	JTTST_EZ(object["verification_level"], this->verificationLevel);
	JTTST_EZ(object["default_message_notifications_level"], this->defaultMessageNotificationsLevel);
	JTTST_EZ(object["explicit_content_filter_level"], this->explicitContentFilterLevel);
	JTTST_EZ(object["mfa_level"], this->mfaLevel);
	JTTST_EZ(object["roles"], this->roles);
	JTTST_EZ(object["emoji"], this->emoji);
	JTTST_EZ(object["features"], this->features);
	JTTST_EZ(object["application_id"], this->applicationID);
	JTTST_EZ(object["widget_enabled"], this->widgetEnabled);
	JTTST_EZ(object["widget_channel_id"], this->widgetChannelID);
	JTTST_EZ(object["system_channel_id"], this->systemChannelID);
	JTTST_EZ(object["joined_at"], this->joinedAt);
	JTTST_EZ(object["large"], this->large);
	JTTST_EZ(object["unavailable"], this->unavailable);
	JTTST_EZ(object["member_count"], this->memberCount);
	JTTST_EZ(object["members"], this->members);
	JTTST_EZ(object["channels"], this->channels);
	JTTST_EZ(object["max_presences"], this->maxPresences);
	JTTST_EZ(object["max_members"], this->maxMembers);
	JTTST_EZ(object["premium_tier"], this->premiumTier);
	JTTST_EZ(object["description"], this->description);
	JTTST_EZ(object["banner_hash"], this->bannerHash);

	return retVal;
}

bool Emoji::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["id"], this->id);
	JTTST_EZ(object["name"], this->name);
	JTTST_EZ(object["require_colons"], this->requireColons);
	JTTST_EZ(object["managed"], this->managed);
	JTTST_EZ(object["animated"], this->animated);

	return retVal;
}

bool Channel::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["id"], this->id);
	JTTST_EZ(object["type"], this->type);
	JTTST_EZ(object["name"], this->name);
	JTTST_EZ(object["topic"], this->topic);
	JTTST_EZ(object["nsfw"], this->nsfw);
	JTTST_EZ(object["icon_hash"], this->iconHash);
	JTTST_EZ(object["guild_id"], this->guildID);
	JTTST_EZ(object["owner_id"], this->ownerID);
	JTTST_EZ(object["application_id"], this->applicationID);
	JTTST_EZ(object["parent_category_id"], this->parentCategoryID);
	JTTST_EZ(object["last_message_id"], this->lastMessageID);
	JTTST_EZ(object["position"], this->position);
	JTTST_EZ(object["permission_overwrites"], this->permissionOverwrites);
	JTTST_EZ(object["user_limit"], this->userLimit);
	JTTST_EZ(object["rate_limit_per_user"], this->rateLimitPerUser);
	JTTST_EZ(object["last_pin_timestamp"], this->lastPinTimestamp);
	JTTST_EZ(object["recipients"], this->recipients);

	return retVal;
}

bool Member::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["nick"], this->nickname);
	JTTST_EZ(object["roles"], this->roles); // This is an array of std::string (Snowflakes), NOT Role
	JTTST_EZ(object["joined_at"], this->joinedAt);
	JTTST_EZ(object["premium_since"], this->premiumSince);
	JTTST_EZ(object["deaf"], this->deaf);
	JTTST_EZ(object["mute"], this->mute);
	JTTST_EZ(object["user"], this->user);

	return retVal;
}

bool UserFlags::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object, this->flags);

	return retVal;
}

bool User::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["id"], this->id);
	JTTST_EZ(object["username"], this->username);
	JTTST_EZ(object["discriminator"], this->discriminator);
	JTTST_EZ(object["avatar_hash"], this->avatarHash);
	JTTST_EZ(object["bot"], this->bot);
	JTTST_EZ(object["mfa_enabled"], this->mfaEnabled);
	JTTST_EZ(object["locale"], this->locale);
	JTTST_EZ(object["email"], this->email);
	JTTST_EZ(object["verified"], this->verified);
	JTTST_EZ(object["flags"], this->flags);
	JTTST_EZ(object["premium_type"], this->premiumType);

	return retVal;
}

bool Role::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["id"], this->id);
	JTTST_EZ(object["name"], this->name);
	JTTST_EZ(object["color"], this->color);
	JTTST_EZ(object["hoist"], this->hoist);
	JTTST_EZ(object["position"], this->position);
	JTTST_EZ(object["permissions"], this->permissions);
	JTTST_EZ(object["managed"], this->managed);
	JTTST_EZ(object["mentionable"], this->mentionable);

	return retVal;
}

bool Message::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["id"], this->id);
	JTTST_EZ(object["channel_id"], this->channelID);
	JTTST_EZ(object["guild_id"], this->guildID);
	JTTST_EZ(object["nonce"], this->nonce);
	JTTST_EZ(object["type"], this->type);
	JTTST_EZ(object["author"], this->author);
	JTTST_EZ(object["content"], this->content);
	JTTST_EZ(object["timestamp"], this->timestamp);
	JTTST_EZ(object["edited_timestamp"], this->editedTimestamp);
	JTTST_EZ(object["tts"], this->tts);
	JTTST_EZ(object["mention_everyone"], this->mentionEveryone);
	//TODO: JTTST_EZ(object["embeds"], this->embeds);
	JTTST_EZ(object["mentionedrole_ids"], this->mentionedroleIDs);
	JTTST_EZ(object["reactions"], this->reactions);
	JTTST_EZ(object["attachments"], this->attachments);
	JTTST_EZ(object["pinned"], this->pinned);
	JTTST_EZ(object["webhook_id"], this->webhookID);

	return retVal;
}

bool Attachment::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["id"], this->id);
	JTTST_EZ(object["filename"], this->filename);
	JTTST_EZ(object["size"], this->size);
	JTTST_EZ(object["url"], this->url);
	JTTST_EZ(object["proxy_url"], this->proxy_url);
	JTTST_EZ(object["width"], this->width);
	JTTST_EZ(object["height"], this->height);

	return retVal;
}

bool Reaction::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["count"], this->count);
	JTTST_EZ(object["me"], this->me);
	JTTST_EZ(object["emoji"], this->emoji);

	return retVal;
}

bool ReadyPacket::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["v"], this->version);
	JTTST_EZ(object["user"], this->user);
	JTTST_EZ(object["private_channels"], this->privateChannels);
	JTTST_EZ(object["guilds"], this->guilds);
	JTTST_EZ(object["session_id"], this->sessionID);
	JTTST_EZ(object["analytics_token"], this->analyticsToken);

	return retVal;
}

bool TypingStartPacket::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["user_id"], this->userID);
	JTTST_EZ(object["guild_id"], this->guildID);
	JTTST_EZ(object["channel_id"], this->channelID);
	JTTST_EZ(object["member"], this->member);
	JTTST_EZ(object["timestamp"], this->timestamp);

	return retVal;
}

bool MessageReactionPacket::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["user_id"], this->userID);
	JTTST_EZ(object["guild_id"], this->guildID);
	JTTST_EZ(object["channel_id"], this->channelID);
	JTTST_EZ(object["message_id"], this->messageID);
	JTTST_EZ(object["emoji"], this->emoji);

	return retVal;
}

bool GuildMemberListUpdatePacket::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["id"], this->id);
	JTTST_EZ(object["guild_id"], this->guildID);
	JTTST_EZ(object["ops"], this->operations);
	JTTST_EZ(object["groups"], this->groups);

	return retVal;
}

bool GuildMemberListUpdateOperation::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["op"], this->op);

	if(object.HasMember("range") && object["range"].IsArray()) {
		auto rangeArray = object["range"].GetArray();
		this->range = std::make_pair<int32_t, int32_t>(
			rangeArray[0].GetInt(), 
			rangeArray[1].GetInt());
	}

	if(object.HasMember("items") && object["items"].IsArray()) {
		auto itemsArray = object["items"].GetArray();

		for (rapidjson::Value &item : itemsArray)
		{
			if (item.IsObject())
			{
				if (item.HasMember("member"))
				{
					Member tmpVar;
					tmpVar.LoadFrom(item["member"]);
					this->items.push_back(tmpVar);
				}
				else if (item.HasMember("group"))
				{
					GuildMemberListGroup tmpVar;
					tmpVar.LoadFrom(item["group"]);
					this->items.push_back(tmpVar);
				}
			}
		}
	}

	return retVal;
}

bool GuildMemberListGroup::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ(object["id"], this->id);
	JTTST_EZ(object["count"], this->count);

	return retVal;
}

//JsonTypeToStructType functions
bool JsonTypeToStructType(rapidjson::Value& object, Discord::JsonDeSerializable &structFieldRef)
{
	return structFieldRef.LoadFrom(object);
}
bool JsonTypeToStructType(rapidjson::Value& object, std::string &structFieldRef)
{
	if(!object.IsString()) {
		return false;
	}

	structFieldRef = object.Get<std::string>();
	
	return true;
}
bool JsonTypeToStructType(rapidjson::Value& object, Discord::Snowflake &structFieldRef)
{
	if (object.IsUint64())
	{
		structFieldRef = Snowflake(object.GetUint64());
		return true;
	}

	if (object.IsString())
	{
		structFieldRef = Snowflake(object.Get<std::string>());
		return true;
	}

	return false;
}
