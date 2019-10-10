#pragma once

#include "pch.h"

namespace Icetrix
{
	class Console
	{
	public:
		Console()
		{
			AllocConsole();
			FILE* pFile = nullptr;
			freopen_s(&pFile, "CONOUT$", "w", stdout);
		}

		~Console()
		{
			fclose((FILE*)stdout);
			HWND hwndConsole = GetConsoleWindow();
			PostMessageW(hwndConsole, WM_CLOSE, 0, 0);
			FreeConsole();
		}
	};
}
