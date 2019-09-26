#include "pch.h"
#include <stdio.h>
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include "imgui\imgui.h"
//#include "imgui\imgui_impl_opengl2.h"
//#include "imgui\imgui_impl_sdl.h"
#include "Menu.h"

bool Menu::OnAttach()
{
	/*
	SDL_Window* sdlWindow = SDL_GL_GetCurrentWindow();
	SDL_GLContext context = SDL_GL_GetCurrentContext();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplSDL2_InitForOpenGL(sdlWindow, context);
	ImGui_ImplOpenGL2_Init();
	*/

    return true;
}

bool Menu::OnUpdate()
{
	/*
	SDL_Window* sdlWindow = SDL_GL_GetCurrentWindow();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(sdlWindow);
	ImGui::NewFrame();

	ImGui::Text("Hello, world!");

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	*/

    return true;
}

void Menu::OnDetach()
{
	/*
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	*/
}