// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "BlackBone/Process/Process.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
//#include "BlackBone/Process/Process.h"
//#include "BlackBone/LocalHook/LocalHook.hpp"
#include "dllmain.h"
#include "Game.h"
#include "OpenGL.h"

using namespace blackbone;

typedef BOOL(__stdcall* f_wglSwapBuffers)(HDC h);
f_wglSwapBuffers o_wglSwapBuffers;
Detour<f_wglSwapBuffers> d_wglSwapBuffers;

float s_ScreenWidth = 1024.f;
float s_ScreenHeight = 768.f;

void DrawESP(playerent* pTargetPlayer, playerent* pLocalPlayer)
{
	Vec3 vHeadPos = pTargetPlayer->m_Head;
	Vec3 vFootPos = pTargetPlayer->m_Position;

	vHeadPos.z += 0.8F; //Regular head pos is not high enough

	Vec3 vScreenHead, vScreenFoot;
	if (OpenGL::WorldToScreen(vHeadPos, &vScreenHead) && OpenGL::WorldToScreen(vFootPos, &vScreenFoot))
	{
		float flHeight = abs(vScreenFoot.y - vScreenHead.y);
		float flWidth = flHeight / 2.0F;

		RenderFunctions::Color32 color = IsVisible(pLocalPlayer->m_Head, pTargetPlayer->m_Head) ? RenderFunctions::Color32(1.0F, 1.0F, 0.0F, 1.0F) : RenderFunctions::Color32(1.0F, 0.0F, 0.0F, 1.0F);

		OpenGL::DrawRect(vScreenHead.x - flWidth / 2, vScreenHead.y, vScreenHead.x + flWidth / 2, vScreenHead.y + flHeight, color, false);
		OpenGL::DrawHealthBar(vScreenHead.x - flWidth / 2, vScreenHead.y - 14, flWidth, 6.0F, static_cast <float>(static_cast <float>(pTargetPlayer->m_Health) / 100.0F), RenderFunctions::Color32(0.0F, 1.0F, 0.0F, 1.0F));
		//OpeNGL::DrawString(vScreenFoot.x - flWidth / 2, vScreenFoot.y + 4, static_cast <int>(color.r * 255), static_cast <int>(color.g * 255), static_cast <int>(color.b * 255), pTargetPlayer->m_Name);
	}
}

/*
void RenderESP()
{
	playerent* localPlayer = *(playerent**)(Game::m_LocalPlayer);
	EntityList* entityList = *reinterpret_cast<EntityList**>(Game::m_EntityList);

	localPlayer->godmode();

	int numberOfPlayers = *(int*)Game::m_NumOfPlayers;
	for (int i = 0; i < numberOfPlayers; ++i)
	{
		auto entity = entityList->entities[i];

		if (entityList && IsValidEntity(entity) && entity->m_Health > 0)
		{
			Vector3 entityHeadPosition = entity->m_Head;
			Vector3 entityScreenPosition = entity->m_Position;

			entityHeadPosition.z += 0.8f;

			float* viewMatrix = (float*)Game::m_ViewMatrix;
			if (OpenGL::WorldToScreen(viewMatrix, entity->m_Position, entityScreenPosition) &&
				OpenGL::WorldToScreen(viewMatrix, entity->m_Head, entityHeadPosition))
			{
				float fHeight = abs(entityScreenPosition.y - entityHeadPosition.y);
				float fWidth = fHeight / 2.0f;

				auto fDistance = OpenGL::GetDistance(localPlayer->m_Head, entity->m_Head);
				auto color = RGB{ 0.f, 0.f, 1.f, 1.f };

				OpenGL::DrawRect(
					entityHeadPosition.x - fWidth / 2,
					entityHeadPosition.y,
					entityHeadPosition.x + fWidth / 2,
					entityHeadPosition.y + fHeight,
					color,
					false
				);

				OpenGL::DrawHealthBar(
					entityHeadPosition.x - fWidth / 2, entityHeadPosition.y - 14,
					fWidth,
					6.0F,
					static_cast <float>(static_cast <float>(entity->m_Health) / 100.0F),
					RGB{0.0F, 1.0F, 0.0F, 1.0F}
				);
			}
		}
	}
}
*/

void SetupOrtho()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void RestoreGL()
{
	glPopMatrix();
	glPopAttrib();
}

BOOL __stdcall h_wglSwapBuffers(HDC h)
{
	SetupOrtho();

	playerent* pLocalPlayer = *(playerent**)(Game::m_LocalPlayer);
	EntityList* entityList = *reinterpret_cast<EntityList**>(Game::m_EntityList);
	int numberOfPlayers = *(int*)Game::m_NumOfPlayers;

	for (int i = 0; i < numberOfPlayers; i++)
	{
		playerent* pPlayer = entityList->entities[i];
		if (!IsValidPtr(pPlayer))
			continue;

		if (pPlayer == pLocalPlayer)
			continue;

		if (pPlayer->m_Health <= 0 || pPlayer->m_Health > 100)
			continue;

		if (pPlayer->m_Head.x == 0 && pPlayer->m_Head.y == 0 && pPlayer->m_Head.z == 0)
			continue;

		DrawESP(pPlayer, pLocalPlayer);
	}

	RestoreGL();

	return d_wglSwapBuffers.CallOriginal(std::move(h));
}

DWORD WINAPI DllThread(LPVOID lpParameter)
{
	Process ac; 
	if (NT_SUCCESS(ac.Attach(GetCurrentProcessId())))
	{
		auto& core = ac.core();
		auto& memory = ac.memory();
		auto& module = ac.modules();
		
		auto mainMod = module.GetMainModule();

		std::cout << "[+] BlackBone attached" << std::endl;

		// DirectX
		/*
		std::cout << "[+] Search for D3D9Device" << std::endl;
		void* d3d9Device[119];
		if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
		{
			std::cout << "[+] Found DirectXDevice vTable at: ";
			std::cout << std::hex << d3d9Device[0] << "\n";

			std::cout << "[+] Trying to hook function..\n";
			//o_Reset = (f_Reset)reset[16];
			o_EndScene = (f_EndScene)d3d9Device[42];

			//typedef HRESULT(__stdcall* f_EndScene)(IDirect3DDevice9* pDevice);

			// Hook endScene to draw in directx9
			if (d_EndScene.Hook((long(__stdcall*)(IDirect3DDevice9*))o_EndScene, (long(__stdcall*)(IDirect3DDevice9*&))h_EndScene, HookType::Int3))
			{
				std::cout << "[+] Hooked EndScene\n";
			}
			else
			{
				std::cout << "[!] Failed to Hook: EndScene\n";
			}
		}
		else
		{
			std::cout << "[!] Search for D3D9Device failed" << std::endl;
		}
		*/

		// OpenGL
		std::cout << "[+] GetModuleHandle for opengl32" << std::endl;
		HMODULE hOpenglModule = GetModuleHandle(L"opengl32.dll");
		std::cout << "[+] Find wglSwapBuffers " << std::endl;
		auto wglSwapBuffersAddr = GetProcAddress(hOpenglModule, "wglSwapBuffers");
		DWORD o_wglSwapBuffers = reinterpret_cast<DWORD>(wglSwapBuffersAddr);

		std::cout << "[+] Hook wglSwapBuffers" << std::endl;
		if (d_wglSwapBuffers.Hook((int(__stdcall*)(HDC))o_wglSwapBuffers, (int(__stdcall*)(HDC&))h_wglSwapBuffers, HookType::Inline))
		{
			std::cout << "[+] Inline hooked wglSwapBuffers\n";
		}
		else
		{
			std::cout << "[!] Failed to Hook: wglSwapBuffers\n";
		}

		std::cout << "[+] Start loop" << std::endl;
		while (!(GetAsyncKeyState(VK_F9) & 1))
		{
			Sleep(100);
		}

		/*
		if (d_EndScene.Restore())
		{
			std::cout << "[+] Restored EndScene" << std::endl;
		}
		*/

		if (d_wglSwapBuffers.Restore())
		{
			std::cout << "[+] Restored wglSwapBuffers" << std::endl;
		}

		std::cout << "[+] End loop" << std::endl;
	}
	else
	{
		std::cout << "[!] Not able to attach BlackBone" << std::endl;
	}

	FreeConsole();
	return 0;
}

Console console;
BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
		CreateThread(0, 0, DllThread, 0, 0, 0);
	}
	
	if (fdwReason == DLL_PROCESS_DETACH)
	{
		FreeLibraryAndExitThread(hinstDLL, 0);
	}
	
	return TRUE;
}
