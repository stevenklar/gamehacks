#include "pch.h"
#include <stdio.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_win32.h>
#include "imgui\imgui_impl_opengl2.h"
//#include "imgui\imgui_impl_sdl.h"
//#include <SDL.h>
//#include <SDL_opengl.h>
#include "Menu.h"

bool Menu::OnAttach()
{
	/*
	SDL_Window* sdlWindow = SDL_GL_GetCurrentWindow();
	SDL_GLContext context = SDL_GL_GetCurrentContext();

	ImGui_ImplSDL2_InitForOpenGL(sdlWindow, context);
	*/

    return true;
}

bool Menu::OnUpdate()
{

    return true;
}

void Menu::OnDetach()
{
}