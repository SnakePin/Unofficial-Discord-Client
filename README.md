# Unofficial-Discord-Client

Requires C++17.

## Implemented functions and objects

- [Gateway] [Implement receive GUILD_MEMBER_LIST_UPDATE](https://github.com/SnakePin/Unofficial-Discord-Client/commit/d59c87c93cb91733d996ef7006376a2ef3e984d5)
- [Gateway] [Implement user-like guild#channel viewing (OP14)](https://github.com/SnakePin/Unofficial-Discord-Client/commit/7c390b018bec4d69daebf012bb96b324d88d74e7)
- [Gateway] [Identifies with a user-like identity packet](https://github.com/SnakePin/Unofficial-Discord-Client/commit/b81f25438b2e4bdcc573da2a05835cd47e2a68ea)
- [Gateway] [Listen for message reaction add/remove](https://github.com/SnakePin/Unofficial-Discord-Client/commit/0a528740359b94ac68689d3b4b978f4260f20612)
- [Gateway] [Login using session RESUME](https://github.com/SnakePin/Unofficial-Discord-Client/commit/3e6e06cbd25608ea5c2540edc28fc0954829fe41)
- [Gateway] [Process MESSAGE_CREATE and TYPING_START](https://github.com/SnakePin/Unofficial-Discord-Client/commit/a37d07280df549555da0b3512fe6b662e9eebbaf)
- [HTTP] [Send *User is typing* to channel](https://github.com/SnakePin/Unofficial-Discord-Client/commit/44872641bbeb11790ff3b26ea584ae84929d4706)
- [Gateway] [Process GUILD_CREATE event](https://github.com/SnakePin/Unofficial-Discord-Client/commit/d2142fa6104b5c158598b7c5edfa4ae7c5586e0d)
- [HTTP] [Send message to channel](https://github.com/SnakePin/Unofficial-Discord-Client/commit/90453b33c1c9882da563ddf021c2f7e464fd7a00)
- [Gateway] [Send heartbeat packet every interval](https://github.com/SnakePin/Unofficial-Discord-Client/commit/4e3d27acf65a3e608330b77901c091cfb47a3033)
- [Gateway] [Respond to HELLO packet with IDENTIFY packet](https://github.com/SnakePin/Unofficial-Discord-Client/commit/de1a98e68269697dacb2c368bd8da9f445755036)
- [HTTP] [Delete message from channel](https://github.com/SnakePin/Unofficial-Discord-Client/commit/59f928a38e2e502ad930a34dd62946af52d75570)
- [HTTP] [Get channel by ID](https://github.com/SnakePin/Unofficial-Discord-Client/commit/59f928a38e2e502ad930a34dd62946af52d75570)
- [HTTP] [Get message by ID](https://github.com/SnakePin/Unofficial-Discord-Client/commit/59f928a38e2e502ad930a34dd62946af52d75570)
- [HTTP] [Get pinned messages in channel](https://github.com/SnakePin/Unofficial-Discord-Client/commit/59f928a38e2e502ad930a34dd62946af52d75570)
- [HTTP] [Unpin a pinned message from channel](https://github.com/SnakePin/Unofficial-Discord-Client/commit/59f928a38e2e502ad930a34dd62946af52d75570)
- [HTTP] [Get current(authenticated) user](https://github.com/SnakePin/Unofficial-Discord-Client/commit/59f928a38e2e502ad930a34dd62946af52d75570)
- [HTTP] [Get user by ID](https://github.com/SnakePin/Unofficial-Discord-Client/commit/59f928a38e2e502ad930a34dd62946af52d75570)

### Represented Discord objects

`Guild, Channel, User, Member, Message, Role, Reaction, Emoji, Embed(stub), Snowflake`

Snowflakes will always be stored internally as `uint64_t`.  
`std::optional` is used to represent fields in these structs whose presence cannot be guaranteed.  
`std::variant` is also used- see `struct GuildMemberListUpdateOperation` for instance.

For represented discord packets, see `discord/packets.hpp` in the include folder. Certain packets are just Discord objects (such as `Message` struct) and do not have a separate packet struct associated with them.

## Useful links

- [Discord websocket protocol](https://discordapp.com/developers/docs/topics/gateway).

## Libraries

### Included in this repo

- [RapidJSON](https://github.com/Tencent/rapidjson/)
- [Asio](https://think-async.com/Asio/)
- [tinyformat](https://github.com/c42f/tinyformat) Useful single-line string construction functions.
- [SDL2](https://www.libsdl.org/) Used for displaying GUI in a cross platform manner.
- [imgui](https://github.com/ocornut/imgui) Used for constructing and managing the GUI.
- [cpprestsdk](https://github.com/Microsoft/cpprestsdk)
- [TCLAP](http://tclap.sourceforge.net/)

### Required libraries

libgl (libgl1-mesa-glx on Debian, mesa-libGL-devel on RHEL)
sdl2, asio, cpprestsdk, tclap, rapidjson should be installed using Vcpkg

## How to build

### Windows(MSVC)

- [Get Vcpkg](https://github.com/Microsoft/vcpkg#quick-start)
- [Install CMake](https://cmake.org/)
- Install a Visual Studio version that is supported by CMake, you can find supported Visual Studio versions from [CMake Documentation](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#visual-studio-generators)
- Execute following commands:
- For dynamic linked 64bit build (recommended):
- `vcpkg install sdl2 asio cpprestsdk[websockets] tclap rapidjson --triplet x64-windows`
- `cd build`
- `cmake .. "-DCMAKE_TOOLCHAIN_FILE=PATH_TO_VCPKG_TOOLCHAIN" "-DVCPKG_TARGET_TRIPLET=x64-windows" -A x64`
- For dynamic linked 32bit build:
- `vcpkg install sdl2 asio cpprestsdk[websockets] tclap rapidjson --triplet x86-windows`
- `cd build`
- `cmake .. "-DCMAKE_TOOLCHAIN_FILE=PATH_TO_VCPKG_TOOLCHAIN" "-DVCPKG_TARGET_TRIPLET=x86-windows" -A Win32`
- Note: For static linked builds, you must create your own triplet file to set CRT linkage to dynamic.
- Release build: `cmake --build . --config Release`
- Debug build: `cmake --build . --config Debug`

Now depending on the configuration, you can find the output files at `output\bin\Debug` or `output\bin\Release`

### Linux

- Install libgl: `apt-get install libgl1-mesa-glx` on Debian, `yum install mesa-libGL-devel` on RHEL based systems
- [Get Vcpkg](https://github.com/Microsoft/vcpkg#quick-start)
- [Install CMake](https://cmake.org/)
- Execute following commands in project root directory:
- `vcpkg install sdl2 asio cpprestsdk[websockets] tclap rapidjson --triplet x64-linux`
- `cd build`
- `cmake .. "-DCMAKE_TOOLCHAIN_FILE=PATH_TO_VCPKG_TOOLCHAIN" "-DVCPKG_TARGET_TRIPLET=x64-linux"`
- Release build: `cmake --build . --config Release`
- Debug build: `cmake --build . --config Debug`

Now you can find output files at `output/bin`
