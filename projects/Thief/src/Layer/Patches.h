
#pragma once

#include "Icetrix/Layer.h"
#include "Icetrix/Process.h"
#include "Icetrix/Memory/BytePatch.h"
#include "BlackBone/Patterns/PatternSearch.h"

class Patches
{
private:
	Icetrix::Application& app;
	blackbone::Process* process;
	Icetrix::Features* features;

	Icetrix::Memory::Patch* unlimitedItemsPatch = nullptr;
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
			{
				blackbone::PatternSearch ps1("\x2B\xDF\x89\x19\x48\x8B\x06");
				std::vector<blackbone::ptr_t> results;
				auto mainModule = process->modules().GetMainModule();
				ps1.Search((void*)mainModule->baseAddress, mainModule->size, results);

				if (results.size() > 0)
				{
					unlimitedItemsPatch = new Icetrix::Memory::Patch{ "Unlimited Items", results.at(0), { 0x2B, 0xDF }, { 0x90, 0x90 } };
					features->Push(new Icetrix::Feature{ "Unlimited Items", false });
				}
			}
		}
    }

	void OnUpdate()
    {
		if (features->Get("Unlimited Items")->enabled && unlimitedItemsPatch)
		{
			Icetrix::Memory::BytePatch::Patch(*unlimitedItemsPatch, 0x0);
		}
		else
		{
			Icetrix::Memory::BytePatch::Unpatch(*unlimitedItemsPatch, 0x0);
		}
    }

	void OnDetach()
	{
		if (unlimitedItemsPatch)
		{
			Icetrix::Memory::BytePatch::Unpatch(*unlimitedItemsPatch, 0x0);
			delete unlimitedItemsPatch;
		}
	}
};
