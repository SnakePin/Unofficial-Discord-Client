#include "imguiClient.hpp"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include "imgui_stdlib.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <unordered_map>
#include <tuple>

typedef std::unordered_map<uint64_t, std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::vector<Discord::Message>>> channelMessageCacheType;

static void cacheGetOrUpdate(std::shared_ptr<MyClient> client, channelMessageCacheType& cache, std::vector<Discord::Message>& outMessages, const Discord::Snowflake& channelID);
static void ImGuiDisplayDiscordMessage(const Discord::Message& message);
static void ImGuiDisplayDiscordMessageBoxAndSendButton(std::shared_ptr<MyClient> client, channelMessageCacheType& cache, const Discord::Snowflake& channelID, std::string& messageToSend);

int startImguiClient(std::shared_ptr<MyClient> client, std::shared_ptr<std::thread> clientThread) {
	std::cout << "Starting SDL2...\n";

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
		std::cout << "SDL_Init error: " << SDL_GetError() << "\n";
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("Unofficial Discord Client GUI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	 // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL2_Init();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	channelMessageCacheType cache;
	std::string messageToSend;

	Discord::User currentUser;
	client->httpAPI.GetCurrentUser(currentUser);

	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				goto CleanupAndExit;
		}

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport();

		ImGui::Begin("Functions");
		{
			if (ImGui::Button("Identify")) {
				client->SendIdentify();
			}
			if (ImGui::Button("Resume")) {
				client->LoadAndSendResume();
			}
			if (ImGui::Button("Call Client::Run() on another thread")) {
				client->Stop();

				//If we don't join the thread here, on the next assignment the thread's deconstructor will be called and it will throw error
				clientThread->join();
				//TODO: use a thread-safe alternative to the line below
				*clientThread = std::thread(&Discord::Client::Run, &*client);
			}
			if (ImGui::Button("Call Client::Stop()")) {
				client->Stop();
			}
		}
		ImGui::End();

		ImGui::Begin("Account");
		{
			if (ImGui::BeginTabBar("Discord")) {
				if (ImGui::BeginTabItem("Guilds")) {
					if (ImGui::BeginTabBar("Guilds", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll)) {
						client->guildsVectorMutex.lock();
						auto guildsCopy = client->guilds;
						client->guildsVectorMutex.unlock();
						for (const Discord::Guild& guild : guildsCopy) {
							if (ImGui::BeginTabItem(guild.name.c_str())) {
								if (ImGui::BeginTabBar(guild.name.c_str(), ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll)) {
									for (const Discord::Channel& channel : guild.channels) {
										if (channel.type == 0 && ImGui::BeginTabItem(channel.name.value_or("Unnamed Channel").c_str())) {
											//TODO: implement the following comment in an actual way
											/*client->OnMessageCreate = [&cache, &channel](Discord::Message m)
											{
												if (cache.find(channel.id.value) != cache.end()) {
													std::get<1>(cache[channel.id.value]).push_back(m);
												}
											};*/
											std::vector<Discord::Message> messages;
											cacheGetOrUpdate(client, cache, messages, channel.id);

											ImGuiDisplayDiscordMessageBoxAndSendButton(client, cache, channel.id, messageToSend);

											for (const Discord::Message& message : messages)
											{
												ImGuiDisplayDiscordMessage(message);
												ImGui::Spacing();
											}

											ImGui::EndTabItem();
										}
									}
									ImGui::EndTabBar();
								}
								ImGui::EndTabItem();
							}
						}
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("DMs")) {
					if (ImGui::BeginTabBar("DMs", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll)) {
						client->privateChannelsVectorMutex.lock();
						auto privateChannelsCopy = client->privateChannels;
						client->privateChannelsVectorMutex.unlock();

						for (const Discord::Channel& channel : privateChannelsCopy) {
							std::string channelName;

							//Group chat
							if (channel.type == 3) {
								channelName = channel.name.value_or("Unnamed Channel");
							}
							//Direct message
							else if (channel.type == 1) {
								if (!channel.recipients.has_value()) {
									continue;
								}
								for (const Discord::User& user : channel.recipients.value()) {
									if (user.id.value != currentUser.id.value) {
										channelName = user.username + "#" + user.discriminator;
									}
								}
							}
							else {
								continue;
							}

							if (ImGui::BeginTabItem(channelName.c_str())) {
								//TODO: implement the following comment in an actual way
								/*client->OnMessageCreate = [&cache, &channel](Discord::Message m)
								{
									if (cache.find(channel.id.value) != cache.end()) {
										std::get<1>(cache[channel.id.value]).push_back(m);
									}
								};*/
								std::vector<Discord::Message> messages;
								cacheGetOrUpdate(client, cache, messages, channel.id);

								ImGuiDisplayDiscordMessageBoxAndSendButton(client, cache, channel.id, messageToSend);

								for (const Discord::Message& message : messages)
								{
									ImGuiDisplayDiscordMessage(message);
									ImGui::Spacing();
								}

								ImGui::EndTabItem();
							}
						}
						ImGui::EndTabBar();

					}
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
		ImGui::End();

		// Rendering
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

CleanupAndExit:

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

static void ImGuiDisplayDiscordMessage(const Discord::Message& message) {
	std::string textToShow;
	if (message.type == 3) {
		textToShow = message.author.username + "#" + message.author.discriminator + " started a discord call.";
	}
	else if (message.type == 0) {
		textToShow = message.author.username + "#" + message.author.discriminator + ": " + message.content;
	}
	ImGui::Text(textToShow.c_str());
	for (const Discord::Attachment& attachment : message.attachments)
	{
		ImGui::Text("^ Attachment:");
		ImGui::Text(("\tFilename: " + attachment.filename).c_str());
		ImGui::Text(("\tSize: " + std::to_string(attachment.size) + " byte(s)").c_str());
		ImGui::Text(("\tURL: " + attachment.url).c_str());
		ImGui::Text(("\tProxy URL: " + attachment.proxy_url).c_str());
	}
}

void ImGuiDisplayDiscordMessageBoxAndSendButton(std::shared_ptr<MyClient> client, channelMessageCacheType& cache, const Discord::Snowflake& channelID, std::string& messageToSend)
{
	//Remove all of these and improve
	std::string usersMessage = messageToSend;
	ImVec2 inputBoxSize(-1.0f, ImGui::GetTextLineHeight() * 4);
	if (ImGui::InputTextMultiline("Type a message: ", &usersMessage, inputBoxSize)) {
		messageToSend = usersMessage;
	}
	if (ImGui::Button("Send message") && !messageToSend.empty()) {
		Discord::CreateMessageParam createMessageParam;
		createMessageParam.tts = false;
		createMessageParam.content = messageToSend;
		client->httpAPI.CreateMessage(channelID, createMessageParam);
		messageToSend = "";

		//TODO: move this code out of this function too and possibly remove this
		//Invalidates cache for this channel
		std::get<0>(cache[channelID.value]) = std::chrono::time_point<std::chrono::system_clock>::min();
	}
}

static void cacheGetOrUpdate(std::shared_ptr<MyClient> client, channelMessageCacheType& cache, std::vector<Discord::Message>& outMessages, const Discord::Snowflake& channelID) {
	if (cache.find(channelID.value) != cache.end()) {
		//TODO: make this async and move this code out of UI thread and out of this function and remove hardcoded values
		auto lastRefresh = std::get<0>(cache[channelID.value]);
		auto timeNow = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = timeNow - lastRefresh;

		if (elapsed_seconds.count() > 60 || lastRefresh == std::chrono::time_point<std::chrono::system_clock>::min()) {
			std::get<0>(cache[channelID.value]) = std::chrono::system_clock::now();
			client->httpAPI.GetMessagesInto(channelID.value, outMessages);
			std::get<1>(cache[channelID.value]) = outMessages;
		}
		else {
			outMessages = std::get<1>(cache[channelID.value]);
		}
	}
	else {
		client->httpAPI.GetMessagesInto(channelID, outMessages);
		cache[channelID.value] =
			std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::vector<Discord::Message>>(std::chrono::system_clock::now(), outMessages);
	}
}
