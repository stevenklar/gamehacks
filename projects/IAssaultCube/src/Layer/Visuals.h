#pragma once

#include "Icetrix/Layer.h"
#include "Icetrix/Platform/OpenGL/OpenGL.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "Game/Game.h"
#include "SDL.h"

typedef void(__stdcall* f_wglSwapBuffers)(HDC h);
typedef void(__cdecl* f_pollEvent)(SDL_Event* e);
typedef void (WINAPI* f_glBindTexture) (GLenum target, GLuint texture);

void StyleDark();
void ApplyDefaultStyle();

class Visuals : public Icetrix::Layer
{
private:
	blackbone::Detour<f_wglSwapBuffers> d_wglSwapBuffers;
	blackbone::Detour<f_pollEvent> d_pollEvent;
	blackbone::Detour<f_glBindTexture> d_glBindTexture;
public:

	bool OnAttach();
	bool OnUpdate();
	void OnDetach();

	bool HookSwapBuffers();
	bool HookPollEvent();
	bool HookBindTexture();

	static void Draw();
	static void ESP(playerent* pTargetPlayer, playerent* pLocalPlayer);
};
