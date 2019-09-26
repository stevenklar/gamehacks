// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Icetrix.h"
#include "Layer/Menu.h"
#include "Layer/Visuals.h"
#include "Layer/NoRecoil.h"
#include "Icetrix/Application.h"

class AssaultCubeInternal : public Icetrix::Application
{
public:
	AssaultCubeInternal()
	{
		// PushLayer(new Overlay()); // Shows broken imgui which you cant do anything with
		// PushLayer(new Aimbot());
		//PushLayer(new Menu());
		PushLayer(new Visuals());
		PushLayer(new NoRecoil());
	}
};

Icetrix::Application* Icetrix::CreateApplication() { return new AssaultCubeInternal(); }
