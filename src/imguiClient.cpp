#include "imguiClient.hpp"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

int startImguiClient(std::shared_ptr<MyClient> client) {
	std::cout << "Starting SDL2...\n";
	
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0){
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL example", 1920 + 1920/2 - WINDOW_WIDTH/2, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	 // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL2_Init();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	char* username = (char*) calloc(33, 1);
	char* password = (char*) calloc(129, 1);

	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
		}

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport();
		
		ImGui::Begin("Login");
		ImGui::InputText("username", username, 32);
		ImGui::InputText("password", password, 128, ImGuiInputTextFlags_Password);
		ImGui::Button("Login");
		ImGui::End();
		
		ImGui::Begin("Account");
		if(ImGui::BeginTabBar("Discord")){
			
			for(const Discord::Guild& guild : client->guilds) {
				if(ImGui::BeginTabItem(guild.name.c_str())) {
					
					if(ImGui::BeginTabBar(guild.name.c_str(), ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll)) {
						
						for(const Discord::Channel& channel : guild.channels) {
							if(channel.type == 0 && ImGui::BeginTabItem(channel.name.value_or("Unnamed Channel").c_str())) {
								ImGui::Text("ayy lmao");
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

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}