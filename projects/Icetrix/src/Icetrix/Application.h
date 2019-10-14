#pragma once

#include "pch.h"
#include "Layer.h"
#include <entt.hpp>

namespace Icetrix
{
    class Application
    {
	private:
		bool panic = false;
	public:
		entt::dispatcher dispatcher{};
	public:
        void Run();
		void Panic();
    };

	Icetrix::Application* CreateApplication();
}
