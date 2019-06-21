# Unofficial-Discord-Client

## Implemented Functions

 - [Gateway] [Process GUILD_CREATE event](https://github.com/SnakePin/Unofficial-Discord-Client/commit/d2142fa6104b5c158598b7c5edfa4ae7c5586e0d)
 - [HTTP] [Send Message to Channel](https://github.com/SnakePin/Unofficial-Discord-Client/commit/90453b33c1c9882da563ddf021c2f7e464fd7a00)
 - [Gateway] [Send Heartbeat packet every interval](https://github.com/SnakePin/Unofficial-Discord-Client/commit/4e3d27acf65a3e608330b77901c091cfb47a3033)
 - [Gateway] [Respond to HELLO packet with IDENTIFY packet](https://github.com/SnakePin/Unofficial-Discord-Client/commit/de1a98e68269697dacb2c368bd8da9f445755036)

### Represented Discord Objects

`Guild, Channel, User, Member, Role, Reaction, Emoji, Embed(stub), Snowflake`

Snowflakes will always be stored internally as `uint64_t`.

## Useful Links

 - [Discord websocket protocol](https://discordapp.com/developers/docs/topics/gateway).

## Libraries

 - [cpr](https://github.com/whoshuu/cpr) for handling HTTP requests.
 - [Simple-Websocket-Server](https://gitlab.com/eidheim/Simple-WebSocket-Server) for handling the gateway websocket client.
 - [RapidJSON](https://github.com/Tencent/rapidjson/)
