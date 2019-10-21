#include "pch.h"
#include "Icetrix/Process.h"
#include "Game/Game.h"
#include "Patches.h"
#include "Icetrix/Feature.h"

void App::Patches::OnAttach()
{
	blackbone::Process* process = Icetrix::Process::GetInstance();
	Icetrix::Features* features = Icetrix::Features::GetInstance();

	if (process->valid())
	{
		auto mainModule = process->modules().GetMainModule();

		for (Icetrix::Memory::Patch patch : patches)
		{
			features->Push(new Icetrix::Feature{ patch.name, false });
		}
	}
	else
	{
		std::cout << "[!] Patches: No valid process found";
	}

	features->Push(new Icetrix::Feature{ "Pseudo Unlimited Health", false });
}

void App::Patches::OnUpdate()
{
	blackbone::Process* process = Icetrix::Process::GetInstance();
	Icetrix::Features* features = Icetrix::Features::GetInstance();

	if (process->valid())
	{
		auto mainModule = process->modules().GetMainModule();

		for (Icetrix::Memory::Patch patch : patches)
		{
			if (features->Get(patch.name)->enabled)
			{
				Icetrix::Memory::BytePatch::Patch(patch, mainModule->baseAddress);
			}
			else
			{
				Icetrix::Memory::BytePatch::Unpatch(patch, mainModule->baseAddress);
			}
		}
	}
	else
	{
		std::cout << "[!] Update Patches: No valid process found";
	}

	if (features->Get("Pseudo Unlimited Health")->enabled)
	{
		Game::GetLocalPlayer()->m_Health = 100000;
		Game::GetLocalPlayer()->m_Vest = 100000;
	}
}

void App::Patches::OnDetach()
{
	blackbone::Process* process = Icetrix::Process::GetInstance();

	if (process->valid())
	{
		auto mainModule = process->modules().GetMainModule();

		for (Icetrix::Memory::Patch patch : patches)
		{
			Icetrix::Memory::BytePatch::Unpatch(patch, mainModule->baseAddress);
		}
	}
	else
	{
		std::cout << "[!] Patches: No valid process found";
	}

	Game::GetLocalPlayer()->m_Health = 100;
	Game::GetLocalPlayer()->m_Vest = 100;
}
