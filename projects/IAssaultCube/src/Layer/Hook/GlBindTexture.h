#pragma once
#include "pch.h"
#include "BlackBone/LocalHook/LocalHook.hpp"

typedef void(__stdcall* f_glBindTexture) (GLenum target, GLuint texture);

namespace Hook
{
	namespace GlBindTextureEvent
	{
		struct Initialize {};
		struct Update {};
	}

	class GlBindTexture
	{
	private:
		Icetrix::Application& app;
		blackbone::Detour<f_glBindTexture> d_glBindTexture;
	public:
		GlBindTexture() : app(Icetrix::Application::GetInstance())
		{
			app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&GlBindTexture::Hook>(*this);
			app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&GlBindTexture::Unhook>(*this);
		}

		void Hook(const Icetrix::LayerEvent::Attach &attach);
		void Unhook(const Icetrix::LayerEvent::Detach &detach);

		static void __stdcall h_glBindTexture(GLenum& target, GLuint& texture);
	};
}
