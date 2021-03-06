#pragma once

#include "pch.h"
#include "Application.h"

static Icetrix::Application* g_Application;

DWORD WINAPI DllThread(LPVOID lpParameter)
{
    auto& app = Icetrix::CreateApplication(); 
	app.Run();

	return 0;
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, DllThread, 0, 0, 0);
    }
	else if (fdwReason == DLL_PROCESS_DETACH)
    {
        FreeLibraryAndExitThread(hinstDLL, 0);
    }

    return TRUE;
}
