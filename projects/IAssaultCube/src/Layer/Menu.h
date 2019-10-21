#pragma once

#include "pch.h"
#include "Icetrix/Application.h"
#include "Icetrix/Layer.h"
#include "Icetrix/Platform/OpenGL/OpenGL.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "Game/Game.h"
#include "SDL.h"
#include "Hook/WglSwapBuffers.h"
#include "Hook/SdlPollEvent.h"

namespace App
{
	class Menu
	{
	private:
		Icetrix::Application& app;
	public:
		Menu() : app(Icetrix::Application::GetInstance())
		{
			app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Menu::Create>(*this);
			app.dispatcher.sink<Icetrix::LayerEvent::Shutdown>().connect<&Menu::Shutdown>(*this);
			app.dispatcher.sink<Hook::WglSwapBufferEvent::Initialize>().connect<&Menu::Init>(*this);
			app.dispatcher.sink<Hook::WglSwapBufferEvent::Update>().connect<&Menu::Render>(*this);
			app.dispatcher.sink<Hook::PollEvent::Update>().connect<&Menu::PollEvent>(*this);
		}

		void Create();
		void Init();
		void Render();
		void PollEvent(const Hook::PollEvent::Update &update);
		void Shutdown();
	};
}
