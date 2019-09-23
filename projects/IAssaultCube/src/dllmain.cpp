// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Icetrix.h"
#include "Layer/Visuals.h"

class AssaultCubeInternal : public Icetrix::Application
{
public:
	AssaultCubeInternal()
	{
		PushLayer(new Visuals());
		PushLayer(new Aimbot());
	}
};

Icetrix::Application* Icetrix::CreateApplication() { return new AssaultCubeInternal(); }
