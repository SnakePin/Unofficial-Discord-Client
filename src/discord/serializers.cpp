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
#define JTTST_EZ(jsonFieldName, memberName) retVal &= JsonTypeToStructType(rapidjson::Pointer(jsonFieldName).Get(object), memberName)

using namespace Discord;

bool Guild::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/id", this->id);
	JTTST_EZ("/name", this->name);
	JTTST_EZ("/splash_hash", this->splashHash);
	JTTST_EZ("/icon_hash", this->iconHash);
	JTTST_EZ("/discovery_splash", this->discoverySplash);
	JTTST_EZ("/owner", this->isCurrentUserOwner);
	JTTST_EZ("/owner_id", this->ownerID);
	JTTST_EZ("/permissions", this->permissions_legacy);
	JTTST_EZ("/permissions_new", this->permissions_new);
	JTTST_EZ("/region", this->region);
	JTTST_EZ("/afk_channel_id", this->afkChannelID);
	JTTST_EZ("/afk_timeout", this->afkTimeout);
	JTTST_EZ("/embedable", this->embedable);
	JTTST_EZ("/embed_channel_id", this->embedChannelID);
	JTTST_EZ("/verification_level", this->verificationLevel);
	JTTST_EZ("/default_message_notifications_level", this->defaultMessageNotificationsLevel);
	JTTST_EZ("/explicit_content_filter_level", this->explicitContentFilterLevel);
	JTTST_EZ("/mfa_level", this->mfaLevel);
	JTTST_EZ("/roles", this->roles);
	JTTST_EZ("/emoji", this->emoji);
	JTTST_EZ("/features", this->features);
	JTTST_EZ("/application_id", this->applicationID);
	JTTST_EZ("/widget_enabled", this->widgetEnabled);
	JTTST_EZ("/widget_channel_id", this->widgetChannelID);
	JTTST_EZ("/system_channel_id", this->systemChannelID);
	JTTST_EZ("/joined_at", this->joinedAt);
	JTTST_EZ("/large", this->large);
	JTTST_EZ("/unavailable", this->unavailable);
	JTTST_EZ("/member_count", this->memberCount);
	JTTST_EZ("/members", this->members);
	JTTST_EZ("/channels", this->channels);
	JTTST_EZ("/max_presences", this->maxPresences);
	JTTST_EZ("/max_members", this->maxMembers);
	JTTST_EZ("/premium_tier", this->premiumTier);
	JTTST_EZ("/description", this->description);
	JTTST_EZ("/banner_hash", this->bannerHash);
	JTTST_EZ("/premium_subscription_count", this->premiumSubscriptionCount);
	JTTST_EZ("/public_updates_channel_id", this->publicUpdatesChannelId);
	JTTST_EZ("/max_video_channel_users", this->maxVideoChannelUsers);
	JTTST_EZ("/approximate_member_count", this->approximateMemberCount);
	JTTST_EZ("/approximate_presence_count", this->approximatePresenceCount);
	JTTST_EZ("/preferred_locale", this->preferredLocale);

	return retVal;
}

bool Emoji::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/id", this->id);
	JTTST_EZ("/name", this->name);
	JTTST_EZ("/require_colons", this->requireColons);
	JTTST_EZ("/managed", this->managed);
	JTTST_EZ("/animated", this->animated);

	return retVal;
}

bool Channel::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/id", this->id);
	JTTST_EZ("/type", this->type);
	JTTST_EZ("/name", this->name);
	JTTST_EZ("/topic", this->topic);
	JTTST_EZ("/nsfw", this->nsfw);
	JTTST_EZ("/icon_hash", this->iconHash);
	JTTST_EZ("/guild_id", this->guildID);
	JTTST_EZ("/owner_id", this->ownerID);
	JTTST_EZ("/application_id", this->applicationID);
	JTTST_EZ("/parent_category_id", this->parentCategoryID);
	JTTST_EZ("/last_message_id", this->lastMessageID);
	JTTST_EZ("/position", this->position);
	JTTST_EZ("/permission_overwrites", this->permissionOverwrites);
	JTTST_EZ("/user_limit", this->userLimit);
	JTTST_EZ("/rate_limit_per_user", this->rateLimitPerUser);
	JTTST_EZ("/last_pin_timestamp", this->lastPinTimestamp);
	JTTST_EZ("/recipients", this->recipients);

	return retVal;
}

bool Member::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/nick", this->nickname);
	JTTST_EZ("/roles", this->roles); // This is an array of std::string (Snowflakes), NOT Role
	JTTST_EZ("/joined_at", this->joinedAt);
	JTTST_EZ("/premium_since", this->premiumSince);
	JTTST_EZ("/deaf", this->deaf);
	JTTST_EZ("/mute", this->mute);
	JTTST_EZ("/user", this->user);

	return retVal;
}

bool UserFlags::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/", this->flags);

	return retVal;
}

bool User::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/id", this->id);
	JTTST_EZ("/username", this->username);
	JTTST_EZ("/discriminator", this->discriminator);
	JTTST_EZ("/avatar", this->avatarHash);
	JTTST_EZ("/bot", this->bot);
	JTTST_EZ("/mfa_enabled", this->mfaEnabled);
	JTTST_EZ("/locale", this->locale);
	JTTST_EZ("/email", this->email);
	JTTST_EZ("/verified", this->verified);
	JTTST_EZ("/flags", this->flags);
	JTTST_EZ("/premium_type", this->premiumType);

	return retVal;
}

bool Role::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/id", this->id);
	JTTST_EZ("/name", this->name);
	JTTST_EZ("/color", this->color);
	JTTST_EZ("/hoist", this->hoist);
	JTTST_EZ("/position", this->position);
	JTTST_EZ("/permissions", this->permissions);
	JTTST_EZ("/managed", this->managed);
	JTTST_EZ("/mentionable", this->mentionable);

	return retVal;
}

bool Message::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/id", this->id);
	JTTST_EZ("/channel_id", this->channelID);
	JTTST_EZ("/guild_id", this->guildID);
	JTTST_EZ("/nonce", this->nonce);
	JTTST_EZ("/type", reinterpret_cast<int32_t&>(this->type));
	JTTST_EZ("/author", this->author);
	JTTST_EZ("/content", this->content);
	JTTST_EZ("/timestamp", this->timestamp);
	JTTST_EZ("/edited_timestamp", this->editedTimestamp);
	JTTST_EZ("/tts", this->tts);
	JTTST_EZ("/mention_everyone", this->mentionEveryone);
	//TODO: JTTST_EZ("/embeds", this->embeds);
	JTTST_EZ("/mentionedrole_ids", this->mentionedroleIDs);
	JTTST_EZ("/reactions", this->reactions);
	JTTST_EZ("/attachments", this->attachments);
	JTTST_EZ("/pinned", this->pinned);
	JTTST_EZ("/webhook_id", this->webhookID);
	JTTST_EZ("/activity", this->activity);
	JTTST_EZ("/message_reference", this->messageReference);

	return retVal;
}

bool Attachment::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/id", this->id);
	JTTST_EZ("/filename", this->filename);
	JTTST_EZ("/size", this->size);
	JTTST_EZ("/url", this->url);
	JTTST_EZ("/proxy_url", this->proxy_url);
	JTTST_EZ("/width", this->width);
	JTTST_EZ("/height", this->height);

	return retVal;
}

bool Reaction::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/count", this->count);
	JTTST_EZ("/me", this->me);
	JTTST_EZ("/emoji", this->emoji);

	return retVal;
}

bool ReadyPacket::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/v", this->version);
	JTTST_EZ("/user", this->user);
	JTTST_EZ("/private_channels", this->privateChannels);
	JTTST_EZ("/guilds", this->guilds);
	JTTST_EZ("/session_id", this->sessionID);
	JTTST_EZ("/analytics_token", this->analyticsToken);

	return retVal;
}

bool TypingStartPacket::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/user_id", this->userID);
	JTTST_EZ("/guild_id", this->guildID);
	JTTST_EZ("/channel_id", this->channelID);
	JTTST_EZ("/member", this->member);
	JTTST_EZ("/timestamp", this->timestamp);

	return retVal;
}

bool MessageReactionPacket::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/user_id", this->userID);
	JTTST_EZ("/guild_id", this->guildID);
	JTTST_EZ("/channel_id", this->channelID);
	JTTST_EZ("/message_id", this->messageID);
	JTTST_EZ("/emoji", this->emoji);

	return retVal;
}

bool GuildMemberListUpdatePacket::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/id", this->id);
	JTTST_EZ("/guild_id", this->guildID);
	JTTST_EZ("/ops", this->operations);
	JTTST_EZ("/groups", this->groups);

	return retVal;
}

bool GuildMemberListUpdateOperation::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/op", this->op);

	if (object.HasMember("range") && object["range"].IsArray()) {
		auto rangeArray = object["range"].GetArray();
		this->range = std::make_pair<int32_t, int32_t>(
			rangeArray[0].GetInt(),
			rangeArray[1].GetInt());
	}

	if (object.HasMember("items") && object["items"].IsArray()) {
		auto itemsArray = object["items"].GetArray();

		for (rapidjson::Value& item : itemsArray)
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

	JTTST_EZ("/id", this->id);
	JTTST_EZ("/count", this->count);

	return retVal;
}

bool MessageActivityObject::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/type", reinterpret_cast<int32_t&>(this->type));
	JTTST_EZ("/party_id", this->partyId);

	return retVal;
}

bool MessageReferenceObject::LoadFrom(rapidjson::Value& object)
{
	bool retVal = true;

	JTTST_EZ("/message_id", this->messageId);
	JTTST_EZ("/channel_id", this->channelId);
	JTTST_EZ("/guildId", this->guildId);

	return retVal;
}

//JsonTypeToStructType functions
bool JsonTypeToStructType(rapidjson::Value* object, Discord::JsonDeSerializable& structFieldRef)
{
	if (object == nullptr) {
		return false;
	}

	return structFieldRef.LoadFrom(*object);
}
bool JsonTypeToStructType(rapidjson::Value* object, std::string& structFieldRef)
{
	if (object == nullptr || !object->IsString()) {
		return false;
	}

	structFieldRef = object->Get<std::string>();

	return true;
}
bool JsonTypeToStructType(rapidjson::Value* object, Discord::Snowflake& structFieldRef)
{
	if (object == nullptr) {
		return false;
	}

	if (object->IsUint64())
	{
		structFieldRef = Snowflake(object->GetUint64());
		return true;
	}

	if (object->IsString())
	{
		structFieldRef = Snowflake(object->Get<std::string>());
		return true;
	}

	return false;
}
