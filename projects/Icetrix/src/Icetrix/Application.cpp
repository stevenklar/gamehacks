#include "pch.h"
#include "Application.h"
#include "Layer.h"
#include "Console.h"

void Icetrix::Application::Run()
{
#ifdef _DEBUG
	Console* console = new Console;
#endif
	dispatcher.trigger<Icetrix::LayerEvent::Attach>();
    
	while (!this->panic)
		dispatcher.trigger<Icetrix::LayerEvent::Update>();

	dispatcher.trigger<Icetrix::LayerEvent::Detach>();

#ifdef _DEBUG
	delete console;
#endif
}

void Icetrix::Application::Panic()
{
	this->panic = true;
}