#pragma once

#include "Icetrix/Application.h"
#include "Icetrix/Layer.h"
#include "Icetrix/Platform/OpenGL/OpenGL.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "Game/Game.h"
#include "SDL.h"

typedef void(__stdcall* f_wglSwapBuffers)(HDC h);
typedef void(__stdcall* f_glBindTexture) (GLenum target, GLuint texture);
typedef void(__cdecl* f_pollEvent)(SDL_Event* e);

class Visuals
{
private:
	blackbone::Detour<f_wglSwapBuffers> d_wglSwapBuffers;
	blackbone::Detour<f_pollEvent> d_pollEvent;
	blackbone::Detour<f_glBindTexture> d_glBindTexture;
public:
	Visuals(Icetrix::Application* app)
	{
		app->dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Visuals::OnAttach>(*this);
		app->dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&Visuals::OnDetach>(*this);
	}

	void OnAttach(const Icetrix::LayerEvent::Attach &attach);
	void OnDetach(const Icetrix::LayerEvent::Detach &detach);

	static void Draw();
	static void ESP(playerent* pTargetPlayer, playerent* pLocalPlayer);

	bool HookSwapBuffers();
	bool HookPollEvent();
	bool HookBindTexture();

	static void __stdcall h_wglSwapBuffers(HDC& h);
	static void __stdcall h_glBindTexture(GLenum& target, GLuint& texture);
	static void __cdecl h_pollEvent(SDL_Event*& event);
};
