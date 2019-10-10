#include <iostream>
#include <Windows.h>
#include "BlackBone/Process/Process.h"
#include <3rd_party/VersionApi.h>
#include <filesystem>

#define LOG(x) std::cout << "[+] " << x << std::endl
#define LOGE(x) std::cout << "[!] " << x << std::endl
#define LOGEW(x) std::wcout << "[!] " << x << std::endl

int main()
{
	LOG("Wait until ac_client.exe is available");
	auto pids = blackbone::Process::EnumByName(L"ac_client.exe");
	while (pids.size() != 1)
	{
		pids = blackbone::Process::EnumByName(L"ac_client.exe");
		if (GetAsyncKeyState(VK_F9) & 1)
		{
			LOG("Stopped");
			return 0;
		}

		Sleep(500);
	}

	LOG("PID:" << pids.front());

	blackbone::Process process;
	if (!NT_SUCCESS(process.Attach(pids.front())))
	{
		LOGE("Could not attach to process");
		return 0;
	}

	//auto dll = L"../../../IAssaultCube//Debug//Win32//IAssaultCube.dll";
	//auto dll = L"C://Users//nop//source//repos//GameHacks//build//bin//IAssaultCube//Debug//Win32//IAssaultCube.dll";
	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);

	const char* dll = "..\\bin\\IAssaultCube\\Release\\Win32\\IAssaultCube.dll";
	std::filesystem::path cwd = std::filesystem::current_path() / dll;
	LOG("Try to inject: ");
	LOG(cwd);
	auto& modules = process.modules();
	auto& modData = modules.Inject(cwd);

	if (!modData)
	{
		LOGE("Could not inject into process");
		/*
		LOG("Unload existing hack and try again");
		modules.Unlink(*modData);
		modules.Unload(*modData);
		*/
	}
	else
	{
		LOG("Successfully injected dll");
	}

	process.Detach();

	std::cin;

	return 0;
}
