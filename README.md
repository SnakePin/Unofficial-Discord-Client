# Unofficial-Discord-Client

Requires C++17.

## Implemented Functions
 - [Gateway] [Message Reaction Add/Remove](https://github.com/SnakePin/Unofficial-Discord-Client/commit/0a528740359b94ac68689d3b4b978f4260f20612)
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

For represented discord packets, see `discord/packets.hpp` in the include folder. Certain packets are just Discord objects (such as `Message` struct) and do not have a separate packet struct associated with them.


## Useful Links
 - [Discord websocket protocol](https://discordapp.com/developers/docs/topics/gateway).

## Libraries

### Included in this repo
 - [cpr](https://github.com/whoshuu/cpr) for handling HTTP(s) requests.
 - [Simple-Websocket-Server](https://gitlab.com/eidheim/Simple-WebSocket-Server) for handling the gateway websocket client.
 - [RapidJSON](https://github.com/Tencent/rapidjson/)
 - [Asio](https://think-async.com/Asio/) required by *Simple-Websocket-Server*.

### Required Libraries
libssl (libssl-dev in Debian, openssl-devel in RHEL)
libcurl (alerady provided by CPR)


## How to build

### Windows(MSVC)
 - Install `Win64 OpenSSL v1.1.1c` or `Win32 OpenSSL v1.1.1c` from https://slproweb.com/products/Win32OpenSSL.html
 - Install [CMake](https://cmake.org/)
 - Install a Visual Studio version that is supported by CMake, you can find supported Visual Studio versions from [CMake Documentation](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#visual-studio-generators)
 - Open command line in project root directory
 - Issue following command `cmake .`
 - Open resulting .sln file in Visual Studio
 - Build the solution

Now depending on your configuration, you can find output files at `output\bin\Debug` or `output\bin\Release`
 
### Linux
 - Install libssl: `apt-get install libssl-dev` in Debian, `yum install openssl-devel` in RHEL
 - Install [CMake](https://cmake.org/)
 - Open command line in project root directory
 - Issue following commands:
 - `cmake .`
 - `make`

Now depending on your configuration, you can find output files at `output\bin`