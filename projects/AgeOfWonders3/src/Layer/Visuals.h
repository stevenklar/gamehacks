#pragma once

#include "Icetrix.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include <d3d11.h>

class Visuals
{
public:
	Visuals(Icetrix::Application* app)
	{
		app->dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Visuals::OnAttach>(*this);
		app->dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&Visuals::OnDetach>(*this);
	}

	void OnAttach(const Icetrix::LayerEvent::Attach& attach)
	{
		auto& modules = Icetrix::Process::GetInstance()->modules();	
	}

	void OnDetach(const Icetrix::LayerEvent::Detach& detach)
	{
	}
};
