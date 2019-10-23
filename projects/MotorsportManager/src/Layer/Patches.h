
#pragma once

#include "Icetrix/Layer.h"
#include "Icetrix/Process.h"
#include "Icetrix/Memory/BytePatch.h"

class Patches
{
private:
	Icetrix::Application& app;
	blackbone::Process* process;
	Icetrix::Features* features;
public:
	Patches() : app(Icetrix::Application::GetInstance()), process(Icetrix::Process::GetInstance()), features(Icetrix::Features::GetInstance())
	{
		app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Patches::OnAttach>(*this);
		app.dispatcher.sink<Icetrix::LayerEvent::Update>().connect<&Patches::OnUpdate>(*this);
		app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&Patches::OnDetach>(*this);
	}

	void OnAttach()
    {
		auto* process = Icetrix::Process::GetInstance();
		auto* features = Icetrix::Features::GetInstance();

		if (process->valid())
		{
			features->Push(new Icetrix::Feature{ "Unlimited Items", false });
			//actor = *reinterpret_cast<Actor**>(process->modules().GetMainModule()->baseAddress + 0x59D6FA0);
		}
    }

	void OnUpdate()
    {
		auto baseAddress = process->modules().GetMainModule()->baseAddress;

		if (features->Get("Unlimited Items")->enabled)
		{
			Icetrix::Memory::BytePatch::Patch(Icetrix::Memory::Patch{ "Unlimited Items", 0xE68C20, { 0x2B, 0xDF }, { 0x90, 0x90 }, 2 }, baseAddress);
		}
		else
		{
			Icetrix::Memory::BytePatch::Unpatch(Icetrix::Memory::Patch{ "Unlimited Items", 0xE68C20, { 0x2B, 0xDF }, { 0x90, 0x90 }, 2 }, baseAddress);
		}
    }

	void OnDetach()
	{
		auto baseAddress = process->modules().GetMainModule()->baseAddress;

		Icetrix::Memory::BytePatch::Unpatch(Icetrix::Memory::Patch{ "Unlimited Items", 0xE68C20, { 0x2B, 0xDF }, { 0x90, 0x90 }, 2 }, baseAddress);
	}
};
