#pragma once

#include "pch.h"
#include "Layer.h"

namespace Icetrix
{
    class Application
    {
    private:
        std::vector<Icetrix::Layer*> layers;
        bool panic = false;
	public:
        void Run();
		void PushLayer(Layer* layer);
    };

	Icetrix::Application* CreateApplication();
}
