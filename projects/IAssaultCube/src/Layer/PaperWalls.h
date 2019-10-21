#pragma once
#include "pch.h"

namespace App
{
	class PaperWalls
	{
	private:
		Icetrix::Application& app;
	public:
		PaperWalls() : app(Icetrix::Application::GetInstance())
		{
			app.dispatcher.sink<Icetrix::Hook::GlBindTextureEvent::Initialize>().connect<&PaperWalls::Register>(*this);
			app.dispatcher.sink<Icetrix::Hook::GlBindTextureEvent::Update>().connect<&PaperWalls::Update>(*this);
		}

		void Register();
		void Update();
	};
}
