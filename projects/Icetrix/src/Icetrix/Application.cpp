#include "Application.h"
#include "Layer.h"

void Icetrix::Application::Run()
{
    for (Layer* layer : layers)
        layer->OnAttach();
    
    while (!panic)
        for (Layer* layer : layers)
            if (!layer->OnUpdate())
                panic = true;

    for (Layer* layer : layers.rbegin)
        layer->OnDetach();
}

void Icetrix::Application::PushLayer(Layer* layer)
{
    layers.push_back(layer);
}