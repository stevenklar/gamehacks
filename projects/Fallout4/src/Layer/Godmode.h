
#pragma once

#include "Icetrix/Layer.h"
#include "Icetrix/Process.h"
#include "Game.h"

class Godmode
{
private:
	Icetrix::Application& app;
	Actor* actor = nullptr;
public:
	Godmode() : app(Icetrix::Application::GetInstance())
	{
		app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Godmode::OnAttach>(*this);
		app.dispatcher.sink<Icetrix::LayerEvent::Update>().connect<&Godmode::OnUpdate>(*this);
		app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&Godmode::OnDetach>(*this);
	}

	void OnAttach()
    {
		auto* process = Icetrix::Process::GetInstance();
		auto* features = Icetrix::Features::GetInstance();

		if (process->valid())
		{
			features->Push(new Icetrix::Feature{ "Godmode", false });
			actor = *reinterpret_cast<Actor**>(process->modules().GetMainModule()->baseAddress + 0x59D6FA0);
		}
    }

	void OnUpdate()
    {
		auto* features = Icetrix::Features::GetInstance();

		if (actor && features->Get("Godmode")->enabled)
		{
			actor->pActorValueInfo.m_HP = 0.0;
			actor->pActorValueInfo.m_RAD = 0.0;
		}
    }

	void OnDetach()
    {
		actor = nullptr;
    }
};
