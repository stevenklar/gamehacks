#pragma once

#include "Icetrix/Layer.h"
#include "Game/Game.h"
#include "Platform/OpenGL/OpenGL.h"

typedef void(__stdcall* f_wglSwapBuffers)(HDC h);

class Visuals : public Icetrix::Layer
{
public:

	bool OnAttach();
	bool OnUpdate();
	void OnDetach();

	bool HookSwapBuffers();
	static void Draw();
	static void ESP(playerent* pTargetPlayer, playerent* pLocalPlayer);
};
