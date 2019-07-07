# Unofficial-Discord-Client

Requires C++17.

## Implemented Functions
 - [Gateway] [Implement receive GUILD_MEMBER_LIST_UPDATE](https://github.com/SnakePin/Unofficial-Discord-Client/commit/d59c87c93cb91733d996ef7006376a2ef3e984d5)
 - [Gateway] [Implement user-like guild#channel viewing (OP14)](https://github.com/SnakePin/Unofficial-Discord-Client/commit/7c390b018bec4d69daebf012bb96b324d88d74e7)
 - [Gateway] [Identifies with a User-like Identity packet](https://github.com/SnakePin/Unofficial-Discord-Client/commit/b81f25438b2e4bdcc573da2a05835cd47e2a68ea)
 - [Gateway] [Listen for Message Reaction Add/Remove](https://github.com/SnakePin/Unofficial-Discord-Client/commit/0a528740359b94ac68689d3b4b978f4260f20612)
 - [Gateway] [Login using session RESUME](https://github.com/SnakePin/Unofficial-Discord-Client/commit/3e6e06cbd25608ea5c2540edc28fc0954829fe41)
 - [Gateway] [Process MESSAGE_CREATE and TYPING_START](https://github.com/SnakePin/Unofficial-Discord-Client/commit/a37d07280df549555da0b3512fe6b662e9eebbaf)
 - [HTTP] [Send *User is typing* to channel](https://github.com/SnakePin/Unofficial-Discord-Client/commit/44872641bbeb11790ff3b26ea584ae84929d4706)
 - [Gateway] [Process GUILD_CREATE event](https://github.com/SnakePin/Unofficial-Discord-Client/commit/d2142fa6104b5c158598b7c5edfa4ae7c5586e0d)
 - [HTTP] [Send Message to Channel](https://github.com/SnakePin/Unofficial-Discord-Client/commit/90453b33c1c9882da563ddf021c2f7e464fd7a00)
 - [Gateway] [Send Heartbeat packet every interval](https://github.com/SnakePin/Unofficial-Discord-Client/commit/4e3d27acf65a3e608330b77901c091cfb47a3033)
 - [Gateway] [Respond to HELLO packet with IDENTIFY packet](https://github.com/SnakePin/Unofficial-Discord-Client/commit/de1a98e68269697dacb2c368bd8da9f445755036)

### Represented Discord Objects

`Guild, Channel, User, Member, Message, Role, Reaction, Emoji, Embed(stub), Snowflake`

Snowflakes will always be stored internally as `uint64_t`.  
`std::optional` is used to represent fields in these structs whose presence cannot be guaranteed.  
`std::variant` is also used - see `struct GuildMemberListUpdateOperation` for instance.

For represented discord packets, see `discord/packets.hpp` in the include folder. Certain packets are just Discord objects (such as `Message` struct) and do not have a separate packet struct associated with them.


## Useful Links

 - [Discord websocket protocol](https://discordapp.com/developers/docs/topics/gateway).

## Libraries

### Included in this repo

 - [cpr](https://github.com/whoshuu/cpr) for handling HTTP requests.
 - [Simple-Websocket-Server](https://gitlab.com/eidheim/Simple-WebSocket-Server) for handling the gateway websocket client.
 - [RapidJSON](https://github.com/Tencent/rapidjson/)
 - [Asio](https://think-async.com/Asio/) required by *Simple-Websocket-Server*.
 - [tinyformat](https://github.com/c42f/tinyformat) Useful single-line string construction functions.

### Required System Libraries

libssl, libcurl. Possibly others.
