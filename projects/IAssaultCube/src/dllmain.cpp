// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "dllmain.h"
#include "Icetrix.h"

using namespace blackbone;

typedef BOOL(__stdcall* f_wglSwapBuffers)(HDC h);
Detour<f_wglSwapBuffers> d_wglSwapBuffers;

void Draw()
{
	Game game;
	playerent* pLocalPlayer = game.GetLocalPlayer();
	playerent** entityList = game.GetEntityList();

	for (int i = 0; i < game.GetNumberOfPlayers(); i++)
	{
		playerent* pPlayer = entityList[i];
		DrawESP(pPlayer, pLocalPlayer);
	}
}

BOOL __stdcall h_wglSwapBuffers(HDC h)
{
	SetupOrtho();
	Draw();
	RestoreGL();
	return d_wglSwapBuffers.CallOriginal(std::move(h));
}

void Run()
{
	Console console;
	Process ac; 

	if (NT_SUCCESS(ac.Attach(GetCurrentProcessId())))
	{
		auto& core = ac.core();
		auto& memory = ac.memory();
		auto& module = ac.modules();
		
		auto mainMod = module.GetMainModule();

		std::cout << "[+] BlackBone attached" << std::endl;

		// OpenGL
		std::cout << "[+] GetModuleHandle for opengl32" << std::endl;
		HMODULE hOpenglModule = GetModuleHandle(L"opengl32.dll");
		std::cout << "[+] Find wglSwapBuffers " << std::endl;
		auto wglSwapBuffersAddr = GetProcAddress((HMODULE)hOpenglModule, "wglSwapBuffers");
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
}
