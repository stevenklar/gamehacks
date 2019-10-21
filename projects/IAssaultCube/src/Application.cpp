#include "pch.h"
#include "Icetrix.h"
#include "Icetrix/Entrypoint.h"

Icetrix::Application& Icetrix::CreateApplication()
{
	auto& app = Icetrix::Application::GetInstance();

	LAYER(Icetrix::Layer::PanicKey);
	LAYER(Icetrix::Hook::WglSwapBuffers);
	LAYER(Icetrix::Hook::SdlPollEvent);
	LAYER(Icetrix::Hook::GlBindTexture);
	LAYER(App::Menu);
	LAYER(App::ESP);
	LAYER(App::Patches);
	LAYER(App::PaperWalls);

	return app;
}
