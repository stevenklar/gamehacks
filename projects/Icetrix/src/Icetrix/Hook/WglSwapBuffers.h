#pragma once
#include <BlackBone/LocalHook/LocalHook.hpp>

typedef void(__stdcall* f_wglSwapBuffers)(HDC h);

namespace Icetrix::Hook
{
	namespace WglSwapBufferEvent
	{
		struct Initialize {};
		struct Update {};
	}

	class WglSwapBuffers
	{
	private:
		blackbone::Detour<f_wglSwapBuffers> d_wglSwapBuffers;
	private:
		Icetrix::Application& app;
	public:
		WglSwapBuffers() : app(Icetrix::Application::GetInstance())
		{
			app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&WglSwapBuffers::Hook>(*this);
			app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&WglSwapBuffers::Unhook>(*this);
		}

		void Hook(const Icetrix::LayerEvent::Attach &attach);
		void Unhook(const Icetrix::LayerEvent::Detach &detach);

		static void __stdcall h_wglSwapBuffers(HDC& h);
	};
}
