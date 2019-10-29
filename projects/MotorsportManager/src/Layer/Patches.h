
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
    }

	void OnUpdate()
    {
    }

	void OnDetach()
	{
	}
};
