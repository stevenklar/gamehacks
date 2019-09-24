#pragma once

#include "Icetrix/Console.h"
#include "Icetrix/Layer.h"
#include "Game/Game.h"
#include "BlackBone/Process/Process.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include "Icetrix/Console.h"
#include "Platform/OpenGL/OpenGL.h"
#include <SDL.h>

typedef BOOL(__stdcall* f_wglSwapBuffers)(HDC h);
typedef int(__cdecl* f_pollEvent)(SDL_Event* e);

class Visuals : public Icetrix::Layer
{
private:
	blackbone::Process process;
	Icetrix::Console console;
public:

	bool OnAttach();
	bool OnUpdate();
	void OnDetach();

	static void Draw();
	static void ESP(playerent* pTargetPlayer, playerent* pLocalPlayer);
};
