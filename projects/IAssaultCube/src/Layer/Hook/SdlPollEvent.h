#pragma once
#include "pch.h"
#include "BlackBone/LocalHook/LocalHook.hpp"

typedef void(__cdecl* f_pollEvent)(SDL_Event* e);

namespace Hook
{
	namespace PollEvent
	{
		struct Update { SDL_Event* event; };
	}

	class SdlPollEvent
	{
	private:
		Icetrix::Application& app;
		blackbone::Detour<f_pollEvent> d_pollEvent;
	public:
		SdlPollEvent() : app(Icetrix::Application::GetInstance())
		{
			app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&SdlPollEvent::Hook>(*this);
			app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&SdlPollEvent::Unhook>(*this);
		}

		void Hook(const Icetrix::LayerEvent::Attach &attach);
		void Unhook(const Icetrix::LayerEvent::Detach &detach);

		static void __cdecl h_pollEvent(SDL_Event*& event);
	};
}
