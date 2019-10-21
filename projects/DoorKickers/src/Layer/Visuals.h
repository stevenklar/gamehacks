#pragma once

#include "Icetrix.h"

class Visuals
{
private:
	Icetrix::Application& app;
public:
	Visuals() : app(Icetrix::Application::GetInstance())
	{
		app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Visuals::OnAttach>(*this);
		app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&Visuals::OnDetach>(*this);
	}

	void OnAttach(const Icetrix::LayerEvent::Attach& attach)
	{
	}

	void OnDetach(const Icetrix::LayerEvent::Detach& detach)
	{
	}
};
