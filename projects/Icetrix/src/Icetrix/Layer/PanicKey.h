#pragma once

#include "pch.h"

namespace Icetrix::Layer
{
	class PanicKey
	{
	private:
		Icetrix::Application& app;
	public:
		PanicKey() : app(Icetrix::Application::GetInstance())
		{
			app.dispatcher.sink<Icetrix::LayerEvent::Update>().connect<&PanicKey::OnUpdate>(*this);
		}

		void OnUpdate(const Icetrix::LayerEvent::Update &update)
		{
			if (GetAsyncKeyState(VK_F9) & 1)
				app.Panic();
		}
	};
}