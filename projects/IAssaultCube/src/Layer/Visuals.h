#pragma once

#include "Icetrix/Layer.h"
#include "Icetrix/Platform/OpenGL/OpenGL.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "Game/Game.h"

typedef void(__stdcall* f_wglSwapBuffers)(HDC h);

class Visuals : public Icetrix::Layer
{
private:
	blackbone::Detour<f_wglSwapBuffers> d_wglSwapBuffers;
public:

	bool OnAttach();
	bool OnUpdate();
	void OnDetach();

	bool HookSwapBuffers();
	static void Draw();
	static void ESP(playerent* pTargetPlayer, playerent* pLocalPlayer);
};
