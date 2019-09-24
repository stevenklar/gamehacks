#pragma once

#include <vector>
#include "Layer.h"

class ImGui : public Icetrix::Layer
{
	bool OnAttach()
	{
		return true;
	}

	bool OnUpdate()
	{
		return true;
	}

	void OnDetach()
	{
	}
};

namespace Icetrix
{
    class Application
    {
    private:
        std::vector<Icetrix::Layer*> layers;
        bool panic = false;
    public:
		Application()
		{
			PushLayer(new ImGui);
		}
        void Run();
		void PushLayer(Layer* layer);
    };

	Icetrix::Application* CreateApplication();
}