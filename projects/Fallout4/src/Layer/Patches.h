#pragma once

#include "Icetrix/Layer.h"
#include "Icetrix/BytePatch.h"

using Icetrix::Memory::Patch;

class Patches
{
private:
	std::vector<Patch>(patches) = {
		Patch{ "Unlimited Ammo", 0x34DBBD, { 0xFF, 0x90, 0x80, 0x07, 0x00, 0x00 }, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, 6 },
		Patch{ "Unlimited AP", 0xDC2D55, { 0xFF, 0x50, 0x30 }, { 0x90, 0x90, 0x90 }, 3 },
		Patch{ "No Weight", 0xD87163, { 0xE8, 0xE8, 0x91, 0x67, 0xFF }, { 0x90, 0x90, 0x90, 0x90, 0x90 }, 5 },
	};
public:
	void OnAttach(const Icetrix::LayerEvent::Attach &attach);
	void OnUpdate(const Icetrix::LayerEvent::Update &update);
	void OnDetach(const Icetrix::LayerEvent::Detach &detach);

	Patches(Icetrix::Application* app)
	{
		app->dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Patches::OnAttach>(*this);
		app->dispatcher.sink<Icetrix::LayerEvent::Update>().connect<&Patches::OnUpdate>(*this);
		app->dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&Patches::OnDetach>(*this);
	}
};
