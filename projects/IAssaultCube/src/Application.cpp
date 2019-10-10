#include "pch.h"
#include "Icetrix/Entrypoint.h"

class AssaultCubeInternal : public Icetrix::Application
{
public:
	AssaultCubeInternal()
	{
		PushLayer(new Visuals());
		PushLayer(new Patches());
	}
};

Icetrix::Application* Icetrix::CreateApplication() { return new AssaultCubeInternal(); }
