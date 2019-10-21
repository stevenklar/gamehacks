#include "pch.h"
#include "Patches.h"
#include "Game.h"

void Patches::OnAttach()
{
	auto* process = Icetrix::Process::GetInstance();
	auto* features = Icetrix::Features::GetInstance();

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
}

void Patches::OnUpdate()
{
	auto* process = Icetrix::Process::GetInstance();
	auto* features = Icetrix::Features::GetInstance();

	if (process->valid())
	{
		auto mainModule = process->modules().GetMainModule();

		for (auto patch : patches)
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

	Actor* actor = *reinterpret_cast<Actor**>(process->modules().GetMainModule()->baseAddress + 0x59D6FA0);
	actor->pActorValueInfo.m_HP = 0.0;
	//actor->pActor->pActorValueInfo->m_HP = 0;
}

void Patches::OnDetach()
{
	auto* process = Icetrix::Process::GetInstance();

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
}
