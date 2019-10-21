#pragma once
#include "pch.h"
#include "Hook/GlBindTexture.h"

namespace App
{
	class PaperWalls
	{
	private:
		Icetrix::Application& app;
	public:
		PaperWalls() : app(Icetrix::Application::GetInstance())
		{
			app.dispatcher.sink<Hook::GlBindTextureEvent::Initialize>().connect<&PaperWalls::RegisterFeature>(*this);
			app.dispatcher.sink<Hook::GlBindTextureEvent::Update>().connect<&PaperWalls::Update>(*this);
		}

		void RegisterFeature(const Hook::GlBindTextureEvent::Initialize& attach);
		void Update(const Hook::GlBindTextureEvent::Update& update);
	};
}
