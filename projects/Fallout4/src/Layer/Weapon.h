
#pragma once

#include "Icetrix/Layer.h"
#include "Icetrix/Process.h"
#include "Game.h"

class Weapon
{
private:
	Icetrix::Application& app;
	blackbone::Process* process;
	Icetrix::Features* features;
public:
	Weapon() : app(Icetrix::Application::GetInstance()), process(Icetrix::Process::GetInstance()), features(Icetrix::Features::GetInstance())
	{
		app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Weapon::OnAttach>(*this);
		app.dispatcher.sink<Icetrix::LayerEvent::Update>().connect<&Weapon::OnUpdate>(*this);
		app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&Weapon::OnDetach>(*this);
	}

	void OnAttach()
    {
		if (process->valid())
		{
			features->Push(new Icetrix::Feature{ "Automatic Weapons", false });
			features->Push(new Icetrix::Feature{ "Rapidfire", false });
			features->Push(new Icetrix::Feature{ "NoRecoil", false });
			features->Push(new Icetrix::Feature{ "NoSpread", false });
		}
    }

	void OnUpdate()
    {
		auto baseAddress = process->modules().GetMainModule()->baseAddress;

		if (features->Get("Automatic Weapons")->enabled)
		{
			Icetrix::Memory::BytePatch::Patch(Patch{ "Automatic Weapons #1", 0xF4CF36, { 0x74, 0x04 }, { 0x90, 0x90 }, 2 }, baseAddress);
			Icetrix::Memory::BytePatch::Patch(Patch{ "Automatic Weapons #1", 0xF4CED8, { 0x76, 0x77 }, { 0x90, 0x90 }, 2 }, baseAddress);
		}
		else
		{
			Icetrix::Memory::BytePatch::Unpatch(Patch{ "Automatic Weapons #1", 0xF4CF36, { 0x74, 0x04 }, { 0x90, 0x90 }, 2 }, baseAddress);
			Icetrix::Memory::BytePatch::Unpatch(Patch{ "Automatic Weapons #1", 0xF4CED8, { 0x76, 0x77 }, { 0x90, 0x90 }, 2 }, baseAddress);
		}

		if (features->Get("Rapidfire")->enabled)
		{
			Icetrix::Memory::BytePatch::Patch(Patch{ "Rapidfire #1", 0xF4CDC6, { 0xF3, 0x0F, 0x10, 0x79, 0x50 }, { 0x0F, 0x57, 0xFF, 0x90, 0x90 }, 5 }, baseAddress);
			Icetrix::Memory::BytePatch::Patch(Patch{ "Rapidfire #2", 0xF4CEAD, { 0xF3, 0x0F, 0x11, 0x4F, 0x50 }, { 0x90, 0x90, 0x90, 0x90, 0x90 }, 5 }, baseAddress);
			Icetrix::Memory::BytePatch::Patch(Patch{ "Rapidfire #3", 0x8A484, { 0x75, 0xEA }, { 0x90, 0x90 }, 2 }, baseAddress);
		}
		else
		{
			Icetrix::Memory::BytePatch::Unpatch(Patch{ "Rapidfire #1", 0xF4CDC6, { 0xF3, 0x0F, 0x10, 0x79, 0x50 }, { 0x0F, 0x57, 0xFF, 0x90, 0x90 }, 5 }, baseAddress);
			Icetrix::Memory::BytePatch::Unpatch(Patch{ "Rapidfire #2", 0xF4CEAD, { 0xF3, 0x0F, 0x11, 0x4F, 0x50 }, { 0x90, 0x90, 0x90, 0x90, 0x90 }, 5 }, baseAddress);
			Icetrix::Memory::BytePatch::Unpatch(Patch{ "Rapidfire #3", 0x8A484, { 0x75, 0xEA }, { 0x90, 0x90 }, 2 }, baseAddress);
		}

		if (features->Get("NoSpread")->enabled)
		{
			Icetrix::Memory::BytePatch::Patch(Patch{ "NoSpread Min", 0x842D8F, { 0xF3, 0x0F, 0x59, 0x43, 0x04 }, { 0x0F, 0x57, 0xC0, 0x90, 0x90 }, 5 }, baseAddress);
			Icetrix::Memory::BytePatch::Patch(Patch{ "NoSpread Max", 0x842DB6, { 0xF3, 0x0F, 0x59, 0x33 }, { 0x0F, 0x57, 0xF6, 0x90 }, 4 }, baseAddress);
		}
		else
		{
			Icetrix::Memory::BytePatch::Unpatch(Patch{ "NoSpread Min", 0x842D8F, { 0xF3, 0x0F, 0x59, 0x43, 0x04 }, { 0x0F, 0x57, 0xC0, 0x90, 0x90 }, 5 }, baseAddress);
			Icetrix::Memory::BytePatch::Unpatch(Patch{ "NoSpread Max", 0x842DB6, { 0xF3, 0x0F, 0x59, 0x33 }, { 0x0F, 0x57, 0xF6, 0x90 }, 4 }, baseAddress);
		}

		if (features->Get("NoRecoil")->enabled)
		{
			Icetrix::Memory::BytePatch::Patch(Patch{ "NoRecoil Min", 0x842946, { 0xF3, 0x44, 0x0F, 0x59, 0x4B, 0x20 }, { 0x45, 0x0F, 0x57, 0xC9, 0x90, 0x90 }, 6 }, baseAddress);
			Icetrix::Memory::BytePatch::Patch(Patch{ "NoRecoil Max", 0x842915, { 0xF3, 0x44, 0x0F, 0x59, 0x43, 0x24 }, { 0x45, 0x0F, 0x57, 0xC0, 0x90, 0x90 }, 6 }, baseAddress);
		}
		else
		{
			Icetrix::Memory::BytePatch::Unpatch(Patch{ "NoRecoil Min", 0x842946, { 0xF3, 0x44, 0x0F, 0x59, 0x4B, 0x20 }, { 0x45, 0x0F, 0x57, 0xC9, 0x90, 0x90 }, 6 }, baseAddress);
			Icetrix::Memory::BytePatch::Unpatch(Patch{ "NoRecoil Max", 0x842915, { 0xF3, 0x44, 0x0F, 0x59, 0x43, 0x24 }, { 0x45, 0x0F, 0x57, 0xC0, 0x90, 0x90 }, 6 }, baseAddress);
		}
    }

	void OnDetach()
    {
		auto baseAddress = process->modules().GetMainModule()->baseAddress;

		Icetrix::Memory::BytePatch::Unpatch(Patch{ "Automatic Weapons #1", 0xF4CF36, { 0x74, 0x04 }, { 0x90, 0x90 }, 2 }, baseAddress);
		Icetrix::Memory::BytePatch::Unpatch(Patch{ "Automatic Weapons #1", 0xF4CED8, { 0x76, 0x77 }, { 0x90, 0x90 }, 2 }, baseAddress);
		Icetrix::Memory::BytePatch::Unpatch(Patch{ "Rapidfire #1", 0xF4CDC6, { 0xF3, 0x0F, 0x10, 0x79, 0x50 }, { 0x0F, 0x57, 0xFF, 0x90, 0x90 }, 5 }, baseAddress);
		Icetrix::Memory::BytePatch::Unpatch(Patch{ "Rapidfire #2", 0xF4CEAD, { 0xF3, 0x0F, 0x11, 0x4F, 0x50 }, { 0x90, 0x90, 0x90, 0x90, 0x90 }, 5 }, baseAddress);
		Icetrix::Memory::BytePatch::Unpatch(Patch{ "Rapidfire #3", 0x8A484, { 0x75, 0xEA }, { 0x90, 0x90 }, 2 }, baseAddress);
		Icetrix::Memory::BytePatch::Unpatch(Patch{ "NoSpread Min", 0x842D8F, { 0xF3, 0x0F, 0x59, 0x43, 0x04 }, { 0x0F, 0x57, 0xC0, 0x90, 0x90 }, 5 }, baseAddress);
		Icetrix::Memory::BytePatch::Unpatch(Patch{ "NoSpread Max", 0x842DB6, { 0xF3, 0x0F, 0x59, 0x33 }, { 0x0F, 0x57, 0xF6, 0x90 }, 4 }, baseAddress);
		Icetrix::Memory::BytePatch::Unpatch(Patch{ "NoRecoil Min", 0x842946, { 0xF3, 0x44, 0x0F, 0x59, 0x4B, 0x20 }, { 0x45, 0x0F, 0x57, 0xC9, 0x90, 0x90 }, 6 }, baseAddress);
		Icetrix::Memory::BytePatch::Unpatch(Patch{ "NoRecoil Max", 0x842915, { 0xF3, 0x44, 0x0F, 0x59, 0x43, 0x24 }, { 0x45, 0x0F, 0x57, 0xC0, 0x90, 0x90 }, 6 }, baseAddress);
    }
};
