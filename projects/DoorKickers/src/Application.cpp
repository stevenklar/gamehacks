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
	}
};

Icetrix::Application* Icetrix::CreateApplication() { return new App(); }
