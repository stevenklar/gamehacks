#pragma once

void Run();

DWORD WINAPI DllThread(LPVOID lpParameter)
{
	Run();
	return 0;
}

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

