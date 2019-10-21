#include "pch.h"
#include "Icetrix/Entrypoint.h"
#include "Icetrix/Layer/PanicKey.h"

Icetrix::Application& Icetrix::CreateApplication()
{
	auto& app = Icetrix::Application::GetInstance();

	LAYER(Icetrix::Layer::PanicKey);
	LAYER(Hook::WglSwapBuffers);
	LAYER(Hook::SdlPollEvent);
	LAYER(Hook::GlBindTexture);
	LAYER(App::Menu);
	LAYER(App::ESP);
	LAYER(App::Patches);
	LAYER(App::PaperWalls);

	return app;
}
