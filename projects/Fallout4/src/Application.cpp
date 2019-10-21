#include "pch.h"
#include "Icetrix/Entrypoint.h"
#include "Icetrix/Layer/PanicKey.h"

Icetrix::Application& Icetrix::CreateApplication()
{
	auto& app = Icetrix::Application::GetInstance();

	LAYER(Icetrix::Layer::PanicKey);
	LAYER(Patches);
	LAYER(Visuals);

	return app;
}
