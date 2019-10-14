#include "pch.h"
#include "Icetrix/Menu/ImGuiDesign.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "imgui.h"
#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_opengl2.h"
#include "Visuals.h"

#pragma comment(lib, "opengl32.lib")

void Visuals::OnAttach(const Icetrix::LayerEvent::Attach &attach)
{
	static HWND hwnd = NULL;
	while (hwnd == NULL)
		hwnd = FindWindowA(NULL, "AssaultCube");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL2_Init();
	Icetrix::Menu::ApplyIcetrixDesign();

	auto features = Icetrix::Features::GetInstance();
	features->Push(new Icetrix::Feature{"ESP", false});
	features->Push(new Icetrix::Feature{"PaperWalls", false});

	HookSwapBuffers();
	HookPollEvent();
	HookBindTexture();
}
	
void Visuals::OnDetach(const Icetrix::LayerEvent::Detach &detach)
{
	if (d_wglSwapBuffers.Restore())
	{
		std::cout << "[+] Restored 'wglSwapBuffers'" << std::endl;
	}
	else
	{
		std::cout << "[!] Failed to restore wglSwapBuffers" << std::endl;
	}

	if (d_pollEvent.Restore())
	{
		std::cout << "[+] Restored 'pollEvent'" << std::endl;
	}
	else
	{
		std::cout << "[!] Failed to restore pollEvent" << std::endl;
	}

	if (d_glBindTexture.Restore())
	{
		std::cout << "[+] Restored 'glBindTexture'" << std::endl;
	}
	else
	{
		std::cout << "[!] Failed to restore glBindTexture" << std::endl;
	}

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool Visuals::HookPollEvent()
{
	HMODULE hSdlModule = GetModuleHandle("sdl.dll");

	if (hSdlModule == NULL)
	{
		std::cout << "[!] Failed to find sdl.dll\n";
		return false;
	}

	auto dPollEvent = GetProcAddress(hSdlModule, "SDL_PollEvent");
	f_pollEvent pollEvent = reinterpret_cast<f_pollEvent>(dPollEvent);

	if (d_pollEvent.Hook(pollEvent, &Visuals::h_pollEvent, blackbone::HookType::HWBP))
	{
		std::cout << "[+] Hooked 'SDL_PollEvent'" << std::endl;
	}
	else
	{
		std::cout << "[!] Failed to Hook: SDL_PollEvent" << std::endl;
		return false;
	}

	return true;
}

bool Visuals::HookSwapBuffers()
{
	HMODULE hOpenglModule = GetModuleHandle("opengl32.dll");

	if (hOpenglModule == NULL)
	{
		std::cout << "[!] Failed to find opengl32.dll::wglSwapBuffers\n";
		return false;
	}

	auto dWglSwapBuffers = GetProcAddress(hOpenglModule, "wglSwapBuffers");
	f_wglSwapBuffers wglSwapBuffers = reinterpret_cast<f_wglSwapBuffers>(dWglSwapBuffers);

	if (d_wglSwapBuffers.Hook(wglSwapBuffers, &Visuals::h_wglSwapBuffers, blackbone::HookType::HWBP))
	{
		std::cout << "[+] Hooked 'wglSwapBuffers'" << std::endl;
	}
	else
	{
		std::cout << "[!] Failed to Hook: wglSwapBuffers" << std::endl;
		return false;
	}

	return true;
}

bool Visuals::HookBindTexture()
{
	HMODULE hModule = GetModuleHandle("opengl32.dll");

	if (hModule == NULL)
	{
		std::cout << "[!] Failed to find opengl32.dll\n";
		return false;
	}

	auto dglBindTexture = GetProcAddress(hModule, "glBindTexture");
	f_glBindTexture glBindTexture = reinterpret_cast<f_glBindTexture>(dglBindTexture);

	if (d_glBindTexture.Hook(glBindTexture, &Visuals::h_glBindTexture, blackbone::HookType::HWBP))
	{
		std::cout << "[+] Hooked 'glBindTexture'" << std::endl;
	}
	else
	{
		std::cout << "[!] Failed to Hook: glBindTexture" << std::endl;
		return false;
	}

	return true;
}

void Visuals::Draw()
{
	playerent* pLocalPlayer = Game::GetLocalPlayer();
	playerent** entityList = Game::GetEntityList();

	for (int i = 0; i < Game::GetNumberOfPlayers(); i++)
	{
		Visuals::ESP(entityList[i], pLocalPlayer);
	}
}

void Visuals::ESP(playerent* pTargetPlayer, playerent* pLocalPlayer)
{
	if (!IsValidPtr(pTargetPlayer))
		return;

	if (pTargetPlayer == pLocalPlayer)
		return;

	if (pTargetPlayer->m_Health <= 0 || pTargetPlayer->m_Health > 100)
		return;

	if (pTargetPlayer->m_Head.x == 0 && pTargetPlayer->m_Head.y == 0 && pTargetPlayer->m_Head.z == 0)
		return;

	Vec3 vHeadPos = pTargetPlayer->m_Head;
	Vec3 vFootPos = pTargetPlayer->m_Position;

	vHeadPos.z += 0.8F; //Regular head pos is not high enough

	Vec3 vScreenHead, vScreenFoot;
	ScreenSettings* pScreenSettings = ScreenSettings::GetInstance();
	if (!IsValidPtr(pScreenSettings))
		return;

	if (OpenGL::WorldToScreen(vHeadPos, &vScreenHead, pScreenSettings->m_Width, pScreenSettings->m_Height)
		&& OpenGL::WorldToScreen(vFootPos, &vScreenFoot, pScreenSettings->m_Width, pScreenSettings->m_Height))
	{
		float flHeight = abs(vScreenFoot.y - vScreenHead.y);
		float flWidth = flHeight / 2.0F;

		auto color = pLocalPlayer->GetPlayerColor(pTargetPlayer);

		OpenGL::DrawRect((int)(vScreenHead.x - flWidth / 2), (int)vScreenHead.y, (int)(vScreenHead.x + flWidth / 2), (int)(vScreenHead.y + flHeight), color, false);
		OpenGL::DrawHealthBar(vScreenHead.x - flWidth / 2, vScreenHead.y - 14, flWidth, 6.0F, static_cast <float>(static_cast <float>(pTargetPlayer->m_Health) / 100.0F), RenderFunctions::Color32(0.0F, 1.0F, 0.0F, 1.0F));
		DrawString(vScreenFoot.x - flWidth / 2, vScreenFoot.y + 4, color.r * 255, color.g * 255, color.b * 255, pTargetPlayer->m_Name);
	}
}

void __stdcall Visuals::h_wglSwapBuffers(HDC& h)
{
	SetupOrtho();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// menu toggle
	static bool bShow = true;
	if (GetAsyncKeyState(VK_INSERT) & 1)
		bShow = !bShow;

	auto features = Icetrix::Features::GetInstance();

	if (bShow)
	{
		SDL_WM_GrabInput(SDL_GrabMode(0));
		SDL_ShowCursor(SDL_TRUE);

		if (ImGui::Begin("Icetrix vagreany unpx", &bShow, ImGuiWindowFlags_AlwaysAutoResize))
		{
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

	if (features->Get("ESP")->enabled)
		Visuals::Draw();

	RestoreGL();
}

void __cdecl Visuals::h_pollEvent(SDL_Event*& event)
{
	ImGuiIO& io = ImGui::GetIO();

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

void WINAPI Visuals::h_glBindTexture(GLenum &target, GLuint &texture)
{
	if (Icetrix::Features::GetInstance()->Get("PaperWalls")->enabled)
		glDisable(GL_DEPTH_TEST);
}
