#include "pch.h"
#include "Application.h"
#include "Layer.h"
#include "Console.h"

void Icetrix::Application::Run()
{
#ifdef _DEBUG
	Console* console = new Console;
#endif

    for (Layer* layer : layers)
        layer->OnAttach();
    
    while (!panic)
        for (Layer* layer : layers)
            if (!layer->OnUpdate())
                panic = true;

    for (Layer* layer : layers)
        layer->OnDetach();

#ifdef _DEBUG
	delete console;
#endif
}

void Icetrix::Application::PushLayer(Layer* layer)
{
    layers.push_back(layer);
}