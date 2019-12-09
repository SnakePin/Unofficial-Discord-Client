#include "imguiClient.hpp"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include "imgui_stdlib.h"
#include "imgui_internal.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "discord/userFlags.hpp"

#include <unordered_map>
#include <tuple>
#include <string>
#include <optional>
#include <atomic>

class ChannelMessageCachingSystem {
	using ChannelMessageCacheItemType = std::tuple<std::chrono::time_point<std::chrono::system_clock>, std::vector<Discord::Message>>;
	using ChannelMessageCacheType = std::unordered_map<uint64_t, ChannelMessageCacheItemType>;
public:
	ChannelMessageCachingSystem(std::shared_ptr<MyClient> _client) :
		client(_client)
	{

	}

	//This function will create cache item entry if an item with the provided channel ID doesn't exist
	const std::vector<Discord::Message>& CacheGetOrUpdate(Discord::Snowflake channelID);

	//This function will create cache item entry if an item with the provided channel ID doesn't exist
	void CacheForceUpdate(Discord::Snowflake channelID);

	//Will return true if the last HTTP API update is 60 seconds ago or if the channel ID doesn't exist
	bool DoesRequireUpdate(Discord::Snowflake channelID);
private:

	ChannelMessageCacheType channelMessageCache;
	std::shared_ptr<MyClient> client;
};


const auto& SystemTimeNow = std::chrono::system_clock::now;

static void ImGuiDisplayDiscordMessage(const Discord::Message& message);
static void ImGuiDisplayDiscordMessageBoxAndSendButton(std::shared_ptr<MyClient> client, ChannelMessageCachingSystem& cache, const Discord::Snowflake& channelID, std::string& messageToSend);
static ImVec4 RGBAToImVec4(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

static void ImGuiTextNoFormat(const char* textStart, const char* textEnd = 0, ImGuiTextFlags_ flags = ImGuiTextFlags_NoWidthForLargeClippedText);
static void ImGuiColoredTextNoFormat(const ImVec4& color, const char* textStart, const char* textEnd = 0, ImGuiTextFlags_ flags = ImGuiTextFlags_NoWidthForLargeClippedText);

template<typename R>
bool is_future_ready(std::future<R> const& f)
{
	return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

int startImguiClient(std::shared_ptr<MyClient> client, std::shared_ptr<std::thread> clientThread) {
	std::cout << "Starting SDL2..." << std::endl;

	const int defaultWindowWidth = 1280;
	const int defaultWindowHeight = 720;
	const char* defaultWindowName = "Unofficial Discord Client GUI";

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_Window* window = SDL_CreateWindow(
		defaultWindowName,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		defaultWindowWidth, defaultWindowHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	 // Enable Keyboard Controls

	io.Fonts->AddFontDefault();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL2_Init();

	ImVec4 clear_color = RGBAToImVec4(115, 140, 153);

	ChannelMessageCachingSystem ourCache(client);

	Discord::User currentUser;
	int currentuserFetchFailCount = 0;
	std::atomic<bool> currentUserFetched = false;
	std::future<bool> currentUserFetchFuture = std::async(std::launch::async, [client, &currentUser, &currentUserFetched]()->bool {
		currentUserFetched = client->httpAPI.GetCurrentUser(currentUser);
		return currentUserFetched;
	});

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
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		/*{
			ImGuiID dockspace_id = ImGui::DockSpaceOverViewport();


			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
			ImGui::DockBuilderAddNode(dockspace_id);
			ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

			ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
			ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
			ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);
			ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);

			ImGui::DockBuilderDockWindow("Functions", dock_id_right);
			ImGui::DockBuilderDockWindow("Profile", dock_id_left);
			ImGui::DockBuilderDockWindow("Chats", dock_id_bottom);
			ImGui::DockBuilderFinish(dockspace_id);
		}*/

		bool apiErrorsDetected = false;

		ImGui::Begin("Functions");
		{
			if (ImGui::Button("Identify")) {
				std::cout << "GUI: Sending identify packet..." << std::endl;
				client->SendIdentify();
			}
			if (ImGui::Button("Call MyClient::LoadAndSendResume")) {
				std::cout << "GUI: Calling MyClient::LoadAndSendResume..." << std::endl;
				client->LoadAndSendResume();
			}
			if (ImGui::Button("Call Client::Run on another thread")) {
				std::cout << "GUI: Stopping client..." << std::endl;
				client->Stop();

				std::cout << "GUI: Joining client thread..." << std::endl;
				//If we don't join the thread here, on the next assignment the thread's deconstructor will be called and it will throw error
				clientThread->join();

				std::cout << "GUI: Creating new client thread and deconstructing the old one..." << std::endl;
				//TODO: use a thread-safe alternative to the line below
				*clientThread = std::thread(&Discord::Client::Run, &*client);
			}
			if (ImGui::Button("Call Client::Stop")) {
				std::cout << "GUI: Stopping client..." << std::endl;
				client->Stop();
				std::cout << "GUI: Stopped client." << std::endl;
			}
			if (ImGui::Button("Call MyClient::UpdateSessionJson")) {
				std::cout << "GUI: Calling MyClient::UpdateSessionJson..." << std::endl;
				client->UpdateSessionJson();
				std::cout << "GUI: MyClient::UpdateSessionJson returned." << std::endl;
			}

			{
				static std::string newToken;
				static int tokenType = 0;
				ImGui::Spacing();
				if (ImGui::Button("Create a new client with the token below")) {
					std::cout << "GUI: Stopping client..." << std::endl;
					client->Stop();
					clientThread->join();

					std::cout << "GUI: Creating new client and thread..." << std::endl;
					//TODO: use a thread-safe alternative to the lines below

					*client = MyClient(newToken, (Discord::AuthTokenType)tokenType);
					*clientThread = std::thread(&Discord::Client::Run, &*client);
				}

				ImGuiTextNoFormat("New token: "); ImGui::SameLine();
				ImGui::InputText("##newtoken", &newToken);
				ImGuiTextNoFormat("New token type: "); ImGui::SameLine();
				ImGui::RadioButton("User token", &tokenType, 0); ImGui::SameLine();
				ImGui::RadioButton("Bot token", &tokenType, 1);
			}

			ImGui::Spacing();
			ImGuiTextNoFormat("Status:");
			ImGui::Text("isIdentified: %d", client->isIdentified.load());
			ImGui::Text("IsRunning(): %d", client->IsRunning());
			ImGui::Text("heartbeatInterval: %d", client->heartbeatInterval);
			ImGui::Text("sequenceNumber: %llu", client->sequenceNumber);
			ImGui::Text("token.tokenType: %s", client->token.tokenType == Discord::AuthTokenType::USER ? "User" : "Bot");
			ImGui::TextWrapped("token.value: %s", client->token.value.c_str());
		}
		ImGui::End();

		ImGui::Begin("Profile");
		{
			if (currentUserFetched) {
				auto displayInfoItem = [](std::string infoName, std::string infoContent) {
					ImGuiTextNoFormat((infoName + ": " + infoContent).c_str());
				};

				displayInfoItem("Username", currentUser.username + "#" + currentUser.discriminator);

				if (currentUser.email.has_value()) {
					displayInfoItem("Email", currentUser.email.value());
				}

				displayInfoItem("Is Email Verified", (currentUser.verified.has_value() && currentUser.verified.value() == true) ? "Yes" : "No");

				std::string nitroInfoString;
				if (currentUser.premiumType.has_value()) {
					nitroInfoString += "Yes(";
					nitroInfoString += (currentUser.premiumType.value() == 1 ? "Nitro Classic" : "Nitro");
					nitroInfoString += ')';
				}
				else {
					nitroInfoString = "No";
				}
				displayInfoItem("Nitro", nitroInfoString);

				if (currentUser.locale.has_value()) {
					displayInfoItem("Locale", currentUser.locale.value());
				}

				displayInfoItem("Is Bot", (currentUser.bot.has_value() && currentUser.bot.value() == true) ? "Yes" : "No");

				if (currentUser.flags.has_value()) {
					Discord::UserFlags& userFlags = currentUser.flags.value();
					if (!userFlags.IsNone()) {
						std::string flagsInfoString;
						if (currentUser.flags.value().IsBugHunter()) {
							flagsInfoString += "(Bug Hunter)";
						}
						if (currentUser.flags.value().IsDiscordEmployee()) {
							flagsInfoString += "(Discord Employee)";
						}
						if (currentUser.flags.value().IsDiscordPartner()) {
							flagsInfoString += "(Discord Partner)";
						}
						if (currentUser.flags.value().IsEarlySupporter()) {
							flagsInfoString += "(Early Supporter)";
						}
						if (currentUser.flags.value().IsHouseBalance()) {
							flagsInfoString += "(House of Balance)";
						}
						if (currentUser.flags.value().IsHouseBravery()) {
							flagsInfoString += "(House of Bravery)";
						}
						if (currentUser.flags.value().IsHouseBrilliance()) {
							flagsInfoString += "(House of Brilliance)";
						}
						if (currentUser.flags.value().IsHypeSquadEvents()) {
							flagsInfoString += "(HypeSquad Events)";
						}
						displayInfoItem("Badges", flagsInfoString);
					}
				}
			}
			else {
				if (currentuserFetchFailCount == 0) {
					ImGuiColoredTextNoFormat(RGBAToImVec4(255, 255, 0), "Fetching current user information...");
				}
				else if (currentuserFetchFailCount <= 15) {
					//Current user is not fetched yet so we check if the task is completed or not
					if (is_future_ready(currentUserFetchFuture) && !currentUserFetchFuture.get()) {
						//Task is completed and it returned false, start new task and increase fail count
						currentUserFetchFuture = std::async(std::launch::async, [client, &currentUser, &currentUserFetched]()->bool {
							currentUserFetched = client->httpAPI.GetCurrentUser(currentUser);
							return currentUserFetched;
						});
						currentuserFetchFailCount++;
					}
					ImGui::TextColored(RGBAToImVec4(255, 0, 0), "Fetching user info failed, trying again. Try: %d", currentuserFetchFailCount);
				}
				else {
					ImGui::TextColored(RGBAToImVec4(255, 255, 0), "Fetching user info failed 15 times, giving up, restart the client.", currentuserFetchFailCount);
				}
			}
		}
		ImGui::End();

		ImGui::Begin("Chats");
		{
			static std::string messageToSend;

			if (ImGui::BeginTabBar("Discord")) {
				if (ImGui::BeginTabItem("Guilds")) {
					if (ImGui::BeginTabBar("Guilds", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll)) {
						client->guildsVectorMutex.lock();
						auto guildsCopy = client->guilds;
						client->guildsVectorMutex.unlock();
						for (const Discord::Guild& guild : guildsCopy) {
							if (guild.name.empty() || guild.id.value == 0) {
								apiErrorsDetected = true;
								continue;
							}
							if (ImGui::BeginTabItem(guild.name.c_str())) {
								if (ImGui::BeginTabBar(guild.name.c_str(), ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll)) {
									for (const Discord::Channel& channel : guild.channels) {

										if ((channel.name.has_value() && channel.name.value().empty()) || channel.id.value == 0) {
											apiErrorsDetected = true;
											continue;
										}

										if (channel.type == 0 && ImGui::BeginTabItem(channel.name.value_or("Unnamed Channel").c_str())) {

											//TODO: implement the following comment in an actual way
											/*client->OnMessageCreate = [&cache, &channel](Discord::Message m)
											{
												if (cache.find(channel.id.value) != cache.end()) {
													std::get<1>(cache[channel.id.value]).push_back(m);
												}
											};*/
											std::vector<Discord::Message> messages = ourCache.CacheGetOrUpdate(channel.id);

											ImGuiDisplayDiscordMessageBoxAndSendButton(client, ourCache, channel.id, messageToSend);

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

							if ((channel.name.has_value() && channel.name.value().empty()) || channel.id.value == 0) {
								apiErrorsDetected = true;
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
								const std::vector<Discord::Message>& messages = ourCache.CacheGetOrUpdate(channel.id);

								ImGuiDisplayDiscordMessageBoxAndSendButton(client, ourCache, channel.id, messageToSend);

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


		if (ImGui::BeginPopup("API Error", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

			ImVec2 popupSize = ImGui::GetWindowSize();

			ImVec2 popupPos = viewport->Size;
			popupPos.x /= 2;
			popupPos.x -= popupSize.x / 2;
			popupPos.y /= 2;
			popupPos.y -= popupSize.y / 2;
			ImGui::SetWindowPos(popupPos);

			ImGuiColoredTextNoFormat(RGBAToImVec4(255, 0, 0), "Discord API errors detected.");
			ImGui::EndPopup();
		}

		if (apiErrorsDetected) {
			ImGui::OpenPopup("API Error");
		}

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

static ImVec4 RGBAToImVec4(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	return ImVec4(r / 255.f, g / 0.f, b / 0.f, a / 255.f);
}

static void ImGuiDisplayDiscordMessage(const Discord::Message& message) {
	std::string textToShow;
	if (message.type == 3) {
		textToShow = message.author.username + "#" + message.author.discriminator + " started a discord call.";
	}
	else if (message.type == 0) {
		textToShow = message.author.username + "#" + message.author.discriminator + ": " + message.content;
	}
	ImGuiTextNoFormat(textToShow.c_str());
	for (const Discord::Attachment& attachment : message.attachments)
	{
		ImGuiColoredTextNoFormat(RGBAToImVec4(255, 0, 0), "^ Attachment:");
		ImGuiTextNoFormat(("\tFilename: " + attachment.filename).c_str());
		ImGuiTextNoFormat(("\tSize: " + std::to_string(attachment.size) + " byte(s)").c_str());
		ImGuiTextNoFormat(("\tURL: " + attachment.url).c_str());
		ImGuiTextNoFormat(("\tProxy URL: " + attachment.proxy_url).c_str());
	}
}

void ImGuiColoredTextNoFormat(const ImVec4& color, const char* textStart, const char* textEnd, ImGuiTextFlags_ flags)
{
	ImGui::PushStyleColor(ImGuiCol_Text, color);
	ImGuiTextNoFormat(textStart, textEnd, flags);
	ImGui::PopStyleColor();
}

void ImGuiTextNoFormat(const char* textStart, const char* textEnd, ImGuiTextFlags_ flags)
{
	ImGui::TextEx(textStart, textEnd, flags);
}

void ImGuiDisplayDiscordMessageBoxAndSendButton(std::shared_ptr<MyClient> client, ChannelMessageCachingSystem& cache, const Discord::Snowflake& channelID, std::string& messageToSend)
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
		cache.CacheForceUpdate(channelID.value);
	}
}

const std::vector<Discord::Message>& ChannelMessageCachingSystem::CacheGetOrUpdate(Discord::Snowflake channelID) {

	//If cache item doesn't exist, DoesRequireUpdate will return true and CacheForceUpdate will create the entry
	if (DoesRequireUpdate(channelID)) {
		CacheForceUpdate(channelID);
	}

	return std::get<1>(channelMessageCache[channelID.value]);
}

void ChannelMessageCachingSystem::CacheForceUpdate(Discord::Snowflake channelID)
{
	if (channelMessageCache.find(channelID.value) == channelMessageCache.end()) {
		//If cache item doesn't exist, we create it
		channelMessageCache[channelID.value] = ChannelMessageCacheItemType();
	}

	//TODO: make this async and move this code out of UI thread
	client->httpAPI.GetMessagesInto(channelID.value, std::get<1>(channelMessageCache[channelID.value]));
	std::get<0>(channelMessageCache[channelID.value]) = SystemTimeNow();
}

bool ChannelMessageCachingSystem::DoesRequireUpdate(Discord::Snowflake channelID)
{
	if (channelMessageCache.find(channelID.value) == channelMessageCache.end()) {
		return true;
	}

	auto lastRefresh = std::get<0>(channelMessageCache[channelID.value]);
	auto timeNow = SystemTimeNow();
	std::chrono::duration<double> elapsed_seconds = timeNow - lastRefresh;

	return elapsed_seconds.count() > 60;
}
