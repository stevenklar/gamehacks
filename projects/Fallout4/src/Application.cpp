#include "pch.h"
#include "Icetrix/Entrypoint.h"

class Fallout4 : public Icetrix::Application
{
public:
	Fallout4()
	{
		PushLayer(new Patches());
		PushLayer(new Visuals());
	}
};

Icetrix::Application* Icetrix::CreateApplication() { return new Fallout4(); }
