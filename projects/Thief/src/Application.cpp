#include "pch.h"
#include "Icetrix/Entrypoint.h"
#include "Icetrix/Layer/PanicKey.h"

Icetrix::Application& Icetrix::CreateApplication()
{
	auto& app = Icetrix::Application::GetInstance();

	new Icetrix::Layer::PanicKey();
	new Icetrix::Hook::DxgiSwapChainPresent(0x4300);
	new Visuals();
	new Patches();

	return app;
}
