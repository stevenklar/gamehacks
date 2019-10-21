#include "pch.h"
#include "Icetrix/Menu/ImGuiDesign.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "imgui.h"
#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_opengl2.h"
#include "ESP.h"

#pragma comment(lib, "opengl32.lib")

void App::ESP::Register()
{
	auto features = Icetrix::Features::GetInstance();
	features->Push(new Icetrix::Feature{ "ESP", false });
}

void App::ESP::Render()
{
	auto features = Icetrix::Features::GetInstance();
	SetupOrtho();

	if (features->Get("ESP")->enabled)
		Draw();

	RestoreGL();
}

void App::ESP::Draw()
{
	playerent* pLocalPlayer = Game::GetLocalPlayer();
	playerent** entityList = Game::GetEntityList();

	for (int i = 0; i < Game::GetNumberOfPlayers(); i++)
	{
		DrawPlayer(entityList[i], pLocalPlayer);
	}
}

void App::ESP::DrawPlayer(playerent* pTargetPlayer, playerent* pLocalPlayer)
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
