#pragma once

#include <vector>
#include "Layer.h"

namespace Icetrix
{
    class Application
    {
    private:
        std::vector<Icetrix::Layer*> layers;
        bool panic = false;
    private:
        Application();
        ~Application();
    public:
        virtual Application* Create();

        void Run();
        void PushLayer(Layer* layer)
    };
}