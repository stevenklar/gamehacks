#pragma once

#include "Application.h"

extern void Application::Create();

DWORD WINAPI DllThread(LPVOID lpParameter)
{
    Application* app = Icetrix::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
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

