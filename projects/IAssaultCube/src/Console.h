#pragma once

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
		FreeConsole();
	}
};
