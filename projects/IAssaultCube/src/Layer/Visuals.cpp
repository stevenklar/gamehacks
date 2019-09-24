#define WINDOWS_IGNORE_PACKING_MISMATCH
#include "pch.h"
#include "Visuals.h"

#pragma comment(lib, "opengl32.lib")
#include "SDL.h"
#include "SDL_events.h"
#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_win32.h"

blackbone::Detour<f_wglSwapBuffers> d_wglSwapBuffers;
BOOL __stdcall h_wglSwapBuffers(HDC h)
{
	// ImGui
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplOpenGL2_NewFrame();

	ImGui::NewFrame();
		bool bShow = true;
		ImGui::ShowDemoWindow(&bShow);
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// ESP
	SetupOrtho();

	Visuals::Draw();
	RestoreGL();
	return d_wglSwapBuffers.CallOriginal(std::move(h));
}

blackbone::Detour<f_pollEvent> d_pollEvent;
int __cdecl h_pollEvent(SDL_Event* event)
{
	static bool showCursor = true;
	if (GetAsyncKeyState(VK_INSERT) & 1)
		showCursor = !showCursor;

	if (showCursor)
	{
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_ShowCursor(SDL_ENABLE);

		ImGuiIO& io = ImGui::GetIO();
		switch (event->type)
		{
		case SDL_BUTTON_LEFT:
		case SDL_BUTTON_RIGHT:
		case SDL_BUTTON_MIDDLE:
		{
			if (event->button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = true;
				std::cout << "[?] event(SDL_BUTTON_LEFT)" << std::endl;
			if (event->button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = true;
				std::cout << "[?] event(SDL_BUTTON_RIGHT)" << std::endl;
			if (event->button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = true;
				std::cout << "[?] event(SDL_BUTTON_MIDDLE)" << std::endl;
			return true;
		}
		}
	}
	else
	{
		SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_ShowCursor(SDL_FALSE);
	}
	return false;
}

bool Visuals::OnAttach()
{
	// imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	HWND hWnd = FindWindow(NULL, L"AssaultCube");
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplOpenGL2_Init();
	ImGui::StyleColorsDark();

	// blackbone
    process.Attach(GetCurrentProcessId());
    if (process.valid())
    {
        /*
        auto& core = process.core();
        auto& memory = process.memory();
        auto& module = process.modules();
        
        auto mainMod = module.GetMainModule();
        */

        std::cout << "[+] BlackBone attached" << std::endl;

		// ---- OPENGL HOOK ----
        std::cout << "[+] GetModuleHandle for opengl32" << std::endl;
        HMODULE hOpenglModule = GetModuleHandle(L"opengl32.dll");

        std::cout << "[+] Find wglSwapBuffers " << std::endl;
        auto wglSwapBuffersAddr = GetProcAddress((HMODULE)hOpenglModule, "wglSwapBuffers");
        DWORD o_wglSwapBuffers = reinterpret_cast<DWORD>(wglSwapBuffersAddr);

        std::cout << "[+] Hook wglSwapBuffers" << std::endl;
        if (d_wglSwapBuffers.Hook((int(__stdcall*)(HDC))o_wglSwapBuffers, (int(__stdcall*)(HDC&))h_wglSwapBuffers, blackbone::HookType::Inline))
        {
            std::cout << "[+] Inline hooked wglSwapBuffers\n";
        }
        else
        {
            std::cout << "[!] Failed to Hook: wglSwapBuffers\n";
			return false;
        }

		// ---- SDL HOOK ----
        std::cout << "[+] GetModuleHandle for SDL" << std::endl;
		HMODULE hSDL = GetModuleHandle(L"sdl.dll");
		if (!hSDL) return false;

        std::cout << "[+] Find pollEvent " << std::endl;
        auto sdlPollEvent = GetProcAddress((HMODULE)hSDL, "SDL_PollEvent");
		if (!sdlPollEvent) return false;

        DWORD o_pollEvent = reinterpret_cast<DWORD>(sdlPollEvent);
        std::cout << "[+] Hook pollEvent" << std::endl;
					  typedef int(__cdecl* f_pollEvent)(SDL_Event* e);
					  // int(__cdecl*)(SDL_Event*)
        if (d_pollEvent.Hook(reinterpret_cast<f_pollEvent>(o_pollEvent), (int(__cdecl*)(SDL_Event * &))(h_pollEvent), blackbone::HookType::Inline, blackbone::CallOrder::HookLast, blackbone::ReturnMethod::UseOriginal))
        //if (true)
        {
            std::cout << "[+] Inline hooked pollEvent\n";
        }
        else
        {
            std::cout << "[!] Failed to Hook: pollEvent\n";
			return false;
        }
    }
    else
    {
        std::cout << "[!] Not able to attach BlackBone" << std::endl;
        return false;
    }

	return true;
}

bool Visuals::OnUpdate()
{
    if (GetAsyncKeyState(VK_F9) & 1)
    {
        // panic
        // should be implemented somewhere else instead, e. g. InputLayer
        return false;
    }

    return true;
}

void Visuals::OnDetach()
{
    if (process.valid())
    {
        if (d_wglSwapBuffers.Restore())
        {
            std::cout << "[+] Restored wglSwapBuffers" << std::endl;
        }
        else
        {
            std::cout << "[!] Failed to restore wglSwapBuffers" << std::endl;
        }

        if (d_pollEvent.Restore())
        {
            std::cout << "[+] Restored pollEvent" << std::endl;
        }
        else
        {
            std::cout << "[!] Failed to restore pollEvent" << std::endl;
        }

        if (NT_ERROR(process.Detach()))
            std::cout << "[!] Not able to detach BlackBone" << std::endl;
    }
    else
    {
        std::cout << "[!] No attached BlackBone found" << std::endl;
    }

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();

    std::cout << "[+] Free Console" << std::endl;
    FreeConsole();
}

void Visuals::Draw()
{
	Game game;
	playerent* pLocalPlayer = game.GetLocalPlayer();
	playerent** entityList = game.GetEntityList();

	pLocalPlayer->godmode();

	for (int i = 0; i < game.GetNumberOfPlayers(); i++)
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
	if (OpenGL::WorldToScreen(vHeadPos, &vScreenHead) && OpenGL::WorldToScreen(vFootPos, &vScreenFoot))
	{
		double flHeight = abs(vScreenFoot.y - vScreenHead.y);
		double flWidth = flHeight / 2.0F;

		RenderFunctions::Color32 color = IsVisible(pLocalPlayer->m_Head, pTargetPlayer->m_Head) ? RenderFunctions::Color32(1.0F, 1.0F, 0.0F, 1.0F) : RenderFunctions::Color32(1.0F, 0.0F, 0.0F, 1.0F);

		OpenGL::DrawRect((int)(vScreenHead.x - flWidth / 2), (int)vScreenHead.y, (int)(vScreenHead.x + flWidth / 2), (int)(vScreenHead.y + flHeight), color, false);
		OpenGL::DrawHealthBar(vScreenHead.x - flWidth / 2, vScreenHead.y - 14, flWidth, 6.0F, static_cast <float>(static_cast <float>(pTargetPlayer->m_Health) / 100.0F), RenderFunctions::Color32(0.0F, 1.0F, 0.0F, 1.0F));
		//OpeNGL::DrawString(vScreenFoot.x - flWidth / 2, vScreenFoot.y + 4, static_cast <int>(color.r * 255), static_cast <int>(color.g * 255), static_cast <int>(color.b * 255), pTargetPlayer->m_Name);
	}
}