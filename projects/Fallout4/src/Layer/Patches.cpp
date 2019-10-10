#include "pch.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "Patches.h"

bool Patches::OnAttach()
{
	blackbone::Process* process = Icetrix::Process::GetInstance();
	Icetrix::Features* features = Icetrix::Features::GetInstance();

	if (process->valid())
	{
		auto mainModule = process->modules().GetMainModule();

		for (Icetrix::Patch patch : patches)
		{
			features->Push(new Icetrix::Feature{ patch.name, false });
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
	blackbone::Process* process = Icetrix::Process::GetInstance();
	Icetrix::Features* features = Icetrix::Features::GetInstance();

    if (GetAsyncKeyState(VK_F9) & 1) // PANIC
    {
        return false;
    }

	if (GetAsyncKeyState(VK_DELETE) & 1)
	{
		for (Icetrix::Patch patch : patches)
		{
			features->Get(patch.name)->Toggle();
		}
	}

	if (process->valid())
	{
		auto mainModule = process->modules().GetMainModule();

		for (Icetrix::Patch patch : patches)
		{
			if (features->Get(patch.name)->enabled)
			{
				Icetrix::BytePatch::Patch(patch, mainModule->baseAddress);
			}
			else
			{
				Icetrix::BytePatch::Unpatch(patch, mainModule->baseAddress);
			}
		}
	}
	else
	{
		std::cout << "[!] Update Patches: No valid process found";
	}

    return true;
}

void Patches::OnDetach()
{
	blackbone::Process* process = Icetrix::Process::GetInstance();

	if (process->valid())
	{
		auto mainModule = process->modules().GetMainModule();

		for (Icetrix::Patch patch : patches)
		{
			Icetrix::BytePatch::Unpatch(patch, mainModule->baseAddress);
		}
	}
	else
	{
		std::cout << "[!] Patches: No valid process found";
	}
}
