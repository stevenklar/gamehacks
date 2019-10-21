#pragma once

#include "pch.h"

namespace App
{
	class ESP
	{
	private:
		Icetrix::Application& app;
	public:
		ESP() : app(Icetrix::Application::GetInstance())
		{
			app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&ESP::Register>(*this);
			app.dispatcher.sink<Icetrix::Hook::WglSwapBufferEvent::Update>().connect<&ESP::Render>(*this);
		}

		void Register();
		void Render();

		static void Draw();
		static void DrawPlayer(playerent* pTargetPlayer, playerent* pLocalPlayer);
	};
}
