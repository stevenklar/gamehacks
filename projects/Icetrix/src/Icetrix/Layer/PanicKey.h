#pragma once

#include "pch.h"

namespace Icetrix::Layer
{
	class PanicKey
	{
	private:
		Icetrix::Application* app;
	public:
		PanicKey(Icetrix::Application* app)
		{
			this->app = app;
			app->dispatcher.sink<Icetrix::LayerEvent::Update>().connect<&PanicKey::OnUpdate>(*this);
		}

		void OnUpdate(const Icetrix::LayerEvent::Update &update)
		{
			if (GetAsyncKeyState(VK_F9) & 1)
				this->app->Panic();
		}
	};
}