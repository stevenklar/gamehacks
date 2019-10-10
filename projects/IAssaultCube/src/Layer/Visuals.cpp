#include "pch.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"
#include "Visuals.h"

#pragma comment(lib, "opengl32.lib")

bool Visuals::OnAttach()
{
	static HWND hwnd = NULL;
	while (hwnd == NULL)
		hwnd = FindWindowA(NULL, "AssaultCube");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL2_Init();
	ImGui::StyleColorsDark();
	//StyleDark();
	ApplyDefaultStyle();

	auto features = Icetrix::Features::GetInstance();
	features->Push(new Icetrix::Feature{"ESP", false});
	features->Push(new Icetrix::Feature{"PaperWalls", false});

	return (HookSwapBuffers() && HookPollEvent() && HookBindTexture());
}

bool Visuals::OnUpdate()
{
    if (GetAsyncKeyState(VK_F9) & 1) // PANIC
    {
        return false;
    }

    return true;
}

void Visuals::OnDetach()
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

void __stdcall h_wglSwapBuffers(HDC& h)
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
		//ImGui::ShowDemoWindow(&bShow);

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

void __cdecl h_pollEvent(SDL_Event*& event)
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

void WINAPI h_glBindTexture(GLenum &target, GLuint &texture)
{
	if (Icetrix::Features::GetInstance()->Get("PaperWalls")->enabled)
		glDisable(GL_DEPTH_TEST);
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

	if (d_pollEvent.Hook(pollEvent, &h_pollEvent, blackbone::HookType::HWBP))
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

	if (d_wglSwapBuffers.Hook(wglSwapBuffers, &h_wglSwapBuffers, blackbone::HookType::HWBP))
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

	if (d_glBindTexture.Hook(glBindTexture, &h_glBindTexture, blackbone::HookType::HWBP))
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
	Game game;
	playerent* pLocalPlayer = game.GetLocalPlayer();
	playerent** entityList = game.GetEntityList();

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

void StyleDark()
{
	ImGuiStyle& st = ImGui::GetStyle();
	st.FrameBorderSize = 1.0f;
	st.FramePadding = ImVec2(4.0f, 2.0f);
	st.ItemSpacing = ImVec2(8.0f, 2.0f);
	st.WindowBorderSize = 1.0f;
	st.TabBorderSize = 1.0f;
	st.WindowRounding = 1.0f;
	st.ChildRounding = 1.0f;
	st.FrameRounding = 1.0f;
	st.ScrollbarRounding = 1.0f;
	st.GrabRounding = 1.0f;
	st.TabRounding = 1.0f;

	// Setup style
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.53f, 0.53f, 0.53f, 0.46f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.85f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 0.53f);
	//colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.48f, 0.47f, 0.47f, 0.91f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.55f, 0.55f, 0.62f);
	colors[ImGuiCol_Button] = ImVec4(0.50f, 0.50f, 0.50f, 0.63f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.67f, 0.67f, 0.68f, 0.63f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.26f, 0.26f, 0.63f);
	colors[ImGuiCol_Header] = ImVec4(0.54f, 0.54f, 0.54f, 0.58f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.64f, 0.65f, 0.65f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
	colors[ImGuiCol_Separator] = ImVec4(0.58f, 0.58f, 0.58f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.87f, 0.87f, 0.87f, 0.53f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
	colors[ImGuiCol_Tab] = ImVec4(0.01f, 0.01f, 0.01f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	//colors[ImGuiCol_DockingPreview] = ImVec4(0.38f, 0.48f, 0.60f, 1.00f);
	//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.77f, 0.33f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.87f, 0.55f, 0.08f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.47f, 0.60f, 0.76f, 0.47f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.58f, 0.58f, 0.58f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void ApplyDefaultStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	//colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
	//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 4.0f;
}

