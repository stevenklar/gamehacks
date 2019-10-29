#include "pch.h"
#include "Icetrix/Entrypoint.h"
#include "Icetrix/Layer/PanicKey.h"

Icetrix::Application& Icetrix::CreateApplication()
{
	auto& app = Icetrix::Application::GetInstance();

	LAYER(Icetrix::Layer::PanicKey);
	new Icetrix::Hook::DxgiSwapChainPresent(0x4300);
	LAYER(Patches);
	LAYER(Visuals);
	LAYER(Godmode);
	LAYER(Weapon);
	LAYER(Watermark);

	return app;
}
