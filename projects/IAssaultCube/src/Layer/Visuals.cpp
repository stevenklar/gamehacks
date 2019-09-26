#include "pch.h"
#include "Visuals.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "Icetrix/Process.h"

#pragma comment(lib, "opengl32.lib")

void __stdcall h_wglSwapBuffers(HDC& h)
{
	SetupOrtho();
	Visuals::Draw();
	RestoreGL();
}

bool Visuals::OnAttach()
{
	return HookSwapBuffers();
}

bool Visuals::OnUpdate()
{
    if (GetAsyncKeyState(VK_F9) & 1)
    {
        // panic
        // should be implemented somewhere else instead, e. g. InputLayer or Menu...
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
		GLfloat flHeight = abs(vScreenFoot.y - vScreenHead.y);
		GLfloat flWidth = flHeight / 2.0F;

		RenderFunctions::Color32 color = IsVisible(pLocalPlayer->m_Head, pTargetPlayer->m_Head) ? RenderFunctions::Color32(1.0F, 1.0F, 0.0F, 1.0F) : RenderFunctions::Color32(1.0F, 0.0F, 0.0F, 1.0F);

		OpenGL::DrawRect((int)(vScreenHead.x - flWidth / 2), (int)vScreenHead.y, (int)(vScreenHead.x + flWidth / 2), (int)(vScreenHead.y + flHeight), color, false);
		OpenGL::DrawHealthBar(vScreenHead.x - flWidth / 2, vScreenHead.y - 14, flWidth, 6.0F, static_cast <float>(static_cast <float>(pTargetPlayer->m_Health) / 100.0F), RenderFunctions::Color32(0.0F, 1.0F, 0.0F, 1.0F));
		//OpeNGL::DrawString(vScreenFoot.x - flWidth / 2, vScreenFoot.y + 4, static_cast <int>(color.r * 255), static_cast <int>(color.g * 255), static_cast <int>(color.b * 255), pTargetPlayer->m_Name);
	}
}