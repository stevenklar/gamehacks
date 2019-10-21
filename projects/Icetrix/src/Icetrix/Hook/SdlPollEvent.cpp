#include "pch.h"
#include "SdlPollEvent.h"

void Icetrix::Hook::SdlPollEvent::Hook(const Icetrix::LayerEvent::Attach& attach)
{
	HMODULE hSdlModule = GetModuleHandle("sdl.dll");

	if (hSdlModule == NULL)
	{
		LOG_ERROR("Failed to find sdl.dll");
		return;
	}

	auto dPollEvent = GetProcAddress(hSdlModule, "SDL_PollEvent");
	f_pollEvent pollEvent = reinterpret_cast<f_pollEvent>(dPollEvent);

	if (d_pollEvent.Hook(pollEvent, &SdlPollEvent::h_pollEvent, blackbone::HookType::HWBP))
		LOG_INFO("Hooked 'SDL_PollEvent'");
	else
		LOG_ERROR("Failed to Hook: SDL_PollEvent");
}

void Icetrix::Hook::SdlPollEvent::Unhook(const Icetrix::LayerEvent::Detach& detach)
{
	if (d_pollEvent.Restore())
		LOG_INFO("Restored 'pollEvent'");
	else
		LOG_ERROR("Failed to restore pollEvent");
}

void __cdecl Icetrix::Hook::SdlPollEvent::h_pollEvent(SDL_Event*& event)
{
	auto& app = Icetrix::Application::GetInstance();
	app.dispatcher.trigger<PollEvent::Update>(event);
}

