#include "pch.h"
#include "Icetrix/Entrypoint.h"
#include "Icetrix/Layer/PanicKey.h"

class App : public Icetrix::Application
{
public:
	App()
	{
		new Icetrix::Layer::PanicKey(this);
		new Visuals(this);
		new Patches(this);
	}
};

Icetrix::Application* Icetrix::CreateApplication() { return new App(); }
