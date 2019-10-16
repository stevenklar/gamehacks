#include "pch.h"
#include "Icetrix/Entrypoint.h"
#include "Icetrix/Layer/PanicKey.h"

class App : public Icetrix::Application
{
public:
	App()
	{
		LAYER(Icetrix::Layer::PanicKey);
		LAYER(Visuals);
		LAYER(Patches);
	}
};

Icetrix::Application* Icetrix::CreateApplication() { return new App(); }
