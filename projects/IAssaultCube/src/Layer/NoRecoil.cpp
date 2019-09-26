#include "NoRecoil.h"
#include <BlackBone/Process/Process.h>
#include <vector>
#include "Game/Game.h"

namespace Offsets
{
	DWORD norecoil = 0x637e9;
}

blackbone::ProcessMemory* memory;

void Patch(const char* feature, blackbone::module_t address, BYTE* patch, int size)
{
	if (NT_SUCCESS(memory->Protect(address, size, PAGE_READWRITE)))
	{
		for (int i = 0; i < size; i++)
		{
			auto newAddress = address + i;
			memory->Write(newAddress, patch[i]);
		}

		std::cout << "[+] Patch '" << feature << "': Wrote " << size << " bytes" << std::endl;
	}
}

bool NoRecoil::OnAttach()
{
	if (blackbone::Process process; NT_SUCCESS(process.Attach(GetCurrentProcessId())))
	{
		memory = &process.memory();
		auto& modules = process.modules();
		auto mainModule = modules.GetMainModule();

		BYTE patch[] = { 0x90, 0x90 };
		Patch("Unlimited Ammo", mainModule->baseAddress + 0x637e9, patch, sizeof(patch));
		BYTE patch2[] = { 0x90, 0x90 };
		Patch("Unlimited Grenades", mainModule->baseAddress + 0x63378, patch2, sizeof(patch2));
		BYTE patch3[] = { 0x90, 0x90 };
		Patch("Rapidfire", mainModule->baseAddress + 0x637e4, patch3, sizeof(patch3));
		BYTE patch4a[] = { 0x31, 0xC0, 0x90, 0x90, 0x90, 0x90, 0x90 };
		Patch("NoRecoil1", mainModule->baseAddress + 0x6226D, patch4a, sizeof(patch4a));
		BYTE patch4b[] = { 0x31, 0xC9, 0x90, 0x90, 0x90, 0x90, 0x90 };
		Patch("NoRecoil2", mainModule->baseAddress + 0x62274, patch4b, sizeof(patch4b));
		byte patch4c[] = { 0x31, 0xc9, 0x90, 0x90, 0x90, 0x90, 0x90 };
		Patch("NoRecoil3_Kickback_primary", mainModule->baseAddress + 0x63dd5, patch4c, sizeof(patch4c));
		byte patch4d[] = { 0x31, 0xc9, 0x90, 0x90, 0x90, 0x90, 0x90 };
		Patch("NoRecoil3_Kickback2_subgun", mainModule->baseAddress + 0x625a4, patch4d, sizeof(patch4d));

		process.Detach();
	}

	return true;
}

bool NoRecoil::OnUpdate()
{
	// godmode
	Game game;
	game.GetLocalPlayer()->m_Health = 1337;
	game.GetLocalPlayer()->m_Vest = 1337;

	return true;
}

void NoRecoil::OnDetach()
{
	if (blackbone::Process process; NT_SUCCESS(process.Attach(GetCurrentProcessId())))
	{
		memory = &process.memory();
		auto& modules = process.modules();
		auto mainModule = modules.GetMainModule();

		BYTE patch[] = { 0xFF, 0x0E };
		Patch("Unlimited Ammo", mainModule->baseAddress + 0x637e9, patch, sizeof(patch));
		BYTE patch2[] = { 0xFF, 0x08 };
		Patch("Unlimited Grenades", mainModule->baseAddress + 0x63378, patch2, sizeof(patch2));
		BYTE patch3[] = { 0x89, 0x0A };
		Patch("Rapidfire", mainModule->baseAddress + 0x637e4, patch3, sizeof(patch3));
		BYTE patch4a[] = { 0x0F, 0xBF, 0x87, 0x20, 0x01, 0x00, 0x00 };
		Patch("NoRecoil1", mainModule->baseAddress + 0x6226D, patch4a, sizeof(patch4a));
		BYTE patch4b[] = { 0x0F, 0xBF, 0x8F, 0x22, 0x01, 0x00, 0x00 };
		Patch("NoRecoil2", mainModule->baseAddress + 0x62274, patch4b, sizeof(patch4b));
		BYTE patch4c[] = { 0x0F, 0xBF, 0x88, 0x16, 0x01, 0x00, 0x00 };
		Patch("NoRecoil3_Kickback_primary", mainModule->baseAddress + 0x63dd5, patch4c, sizeof(patch4c));
		BYTE patch4d[] = { 0x0F, 0xBF, 0x88, 0x16, 0x01, 0x00, 0x00 };
		Patch("NoRecoil3_Kickback2_subgun", mainModule->baseAddress + 0x625a4, patch4d, sizeof(patch4d));
		

		process.Detach();
	}
}

