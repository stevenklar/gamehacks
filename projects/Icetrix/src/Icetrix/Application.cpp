#include "Application.h"
#include "Layer.h"
#include "Console.h"

void Icetrix::Application::Run()
{
	//Console* console = new Console;

    for (Layer* layer : layers)
        layer->OnAttach();
    
    while (!panic)
        for (Layer* layer : layers)
            if (!layer->OnUpdate())
                panic = true;

    for (Layer* layer : layers)
        layer->OnDetach();

	//delete console;
}

void Icetrix::Application::PushLayer(Layer* layer)
{
    layers.push_back(layer);
}