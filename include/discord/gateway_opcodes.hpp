#pragma once

namespace Discord
{
    enum GatewayOpcodes 
    {
        /// Server to client - Used to send most events.
        Dispatch = 0,
        /// Both ways - Used to keep the connection alive and measure latency.
        Heartbeat = 1,
        /// Client to server - Used to associate a connection with a token and specify configuration.
        Identify = 2,
        /// Client to server - Used to update client's status and current game id.
        StatusUpdate = 3,
        /// Client to server - Used to join a particular voice channel.
        VoiceStateUpdate = 4,
        /// Client to server - Used to ensure the guild's voice server is alive.
        VoiceServerPing = 5,
        /// Client to server - Used to resume a connection after a redirect occurs.
        Resume = 6,
        /// Server to client - Used to notify a client that they must reconnect to another gateway.
        Reconnect = 7,
        /// Client to server - Used to request members that were withheld by large_threshold
        RequestGuildMembers = 8,
        /// Server to client - Used to notify the client that their session has expired and cannot be resumed.
        InvalidSession = 9,
        /// Server to client - Used to provide information to the client immediately on connection.
        Hello = 10,
        /// Server to client - Used to reply to a client's heartbeat.
        HeartbeatAck = 11,
        /// Client to server - Used to request presence updates from particular guilds.
        GuildSync = 12
    };
}
