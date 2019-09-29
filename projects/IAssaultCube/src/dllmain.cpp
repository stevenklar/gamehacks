// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Icetrix.h"
#include "Layer/Visuals.h"
#include "Layer/Patches.h"
#include "Layer/Aimbot.h"
#include "Icetrix/Application.h"

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
