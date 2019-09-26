#include <vector>
#include <BlackBone/Process/Process.h>
#include "Game/Game.h"
#include "Icetrix/Process.h"
#include "Patches.h"

bool Patches::OnAttach()
{
	blackbone::Process* process = Icetrix::Process::GetIntance();
	if (process->valid())
	{
		auto mainModule = process->modules().GetMainModule();

		for (GamePatch patch : patches)
		{
			Patch(patch, mainModule->baseAddress);
		}
	}
	else
	{
		std::cout << "[!] Patches: No valid process found";
	}

	return true;
}

bool Patches::OnUpdate()
{
	// godmode
	Game game;
	game.GetLocalPlayer()->m_Health = 133337;
	game.GetLocalPlayer()->m_Vest = 133337;

	return true;
}

void Patches::OnDetach()
{
	blackbone::Process* process = Icetrix::Process::GetIntance();
	if (process->valid())
	{
		auto mainModule = process->modules().GetMainModule();

		for (GamePatch patch : patches)
		{
			Unpatch(patch, mainModule->baseAddress);
		}
	}
	else
	{
		std::cout << "[!] Patches: No valid process found";
	}

	Game game;
	game.GetLocalPlayer()->m_Health = 100;
	game.GetLocalPlayer()->m_Vest = 100;
}

void Patches::Patch(GamePatch patch, blackbone::module_t baseAddress)
{
	blackbone::ProcessMemory* memory = &(Icetrix::Process::GetIntance())->memory();

	if (NT_SUCCESS(memory->Protect(baseAddress + patch.address, patch.size, PAGE_READWRITE)))
	{
		for (int i = 0; i < patch.size; i++)
		{
			auto newAddress = baseAddress + patch.address + i;
			memory->Write(newAddress, patch.patch[i]);
		}

		std::cout << "[+] Patched '" << patch.name << "': Wrote " << patch.size << " bytes" << std::endl;
	}
}

void Patches::Unpatch(GamePatch patch, blackbone::module_t baseAddress)
{
	blackbone::ProcessMemory* memory = &(Icetrix::Process::GetIntance())->memory();

	if (NT_SUCCESS(memory->Protect(baseAddress + patch.address, patch.size, PAGE_READWRITE)))
	{
		for (int i = 0; i < patch.size; i++)
		{
			auto newAddress = baseAddress + patch.address + i;
			memory->Write(newAddress, patch.original[i]);
		}

		std::cout << "[+] Unpatched '" << patch.name << "': Wrote " << patch.size << " bytes" << std::endl;
	}
}

