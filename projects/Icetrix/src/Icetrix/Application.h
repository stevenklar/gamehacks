#pragma once

#include "pch.h"
#include "Layer.h"
#include <entt.hpp>

namespace Icetrix
{
    class Application
    {
	public:
		entt::dispatcher dispatcher{};
	public:
		static Application& GetInstance()
		{
			static Application app;
			return app;
		}

        void Run();
		void Panic();
	private:
		bool panic = false;
		Application() {};
	public:
		Application(Application const&) = delete;
    };
}
