#include "pch.h"
#include "Icetrix/Menu/ImGuiDesign.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "imgui.h"
#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_opengl2.h"
#include "Menu.h"

#pragma comment(lib, "opengl32.lib")

void App::Menu::Create()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
}

void App::Menu::Init()
{
	static HWND hwnd = NULL;
	while (hwnd == NULL)
		hwnd = FindWindowA(NULL, "AssaultCube");

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL2_Init();
	Icetrix::Menu::ApplyIcetrixDesign();
}
	
void App::Menu::Render()
{
	auto features = Icetrix::Features::GetInstance();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool bShow = true;
	if (GetAsyncKeyState(VK_INSERT) & 1)
		bShow = !bShow;

	if (bShow)
	{
		SDL_WM_GrabInput(SDL_GrabMode(0));
		SDL_ShowCursor(SDL_TRUE);

		if (ImGui::Begin("Icetrix vagreany unpx", &bShow, ImGuiWindowFlags_AlwaysAutoResize))
		{
#ifdef DEBUG
			ImGuiIO &io = ImGui::GetIO();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			// Proof of Concept :)
			int ammoAddress = entt::monostate<"ammoAddress"_hs>{};
			if (ammoAddress != 0)
				ImGui::SliderInt("Ammo", (int*)ammoAddress, 0, 1337);

			int grenadeAddress = entt::monostate<"grenadeAddress"_hs>{};
			if (grenadeAddress != 0)
				ImGui::SliderInt("Grenades", (int*)grenadeAddress, 0, 1337);
#endif

			if (ImGui::Button("Activate"))
			{
				// TODO: Replace with Features class method
				for (auto feature : features->All())
				{
					feature->enabled = true;
				}
			}

			if (ImGui::Button("Deactivate"))
			{
				for (auto feature : features->All())
				{
					feature->enabled = false;
				}
			}

			for (auto feature : features->All())
			{
				ImGui::Checkbox(feature->label, &feature->enabled);
			}

			ImGui::End();
		}
	}
	else
	{
		SDL_WM_GrabInput(SDL_GrabMode(1));
		SDL_ShowCursor(SDL_FALSE);
	}

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void App::Menu::PollEvent(const Icetrix::Hook::PollEvent::Update& update)
{
	ImGuiIO& io = ImGui::GetIO();
	auto event = update.event;

	switch (event->type)
	{
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEBUTTONDOWN:
	{
		if (event->button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = !io.MouseDown[0];
		if (event->button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = !io.MouseDown[1];
		if (event->button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = !io.MouseDown[2];
	}
	break;
	}
}

void App::Menu::Shutdown()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
