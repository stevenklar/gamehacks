#pragma once

#include "Icetrix/Console.h"
#include "Icetrix/Layer.h"

#include "BlackBone/Process/Process.h"
#include "BlackBone/LocalHook/LocalHook.hpp"

#include "OpenGL.h"
#include "Game.h"
#include "ESP.h"
#include "Console.h"


typedef BOOL(__stdcall* f_wglSwapBuffers)(HDC h);

class Visuals : public Icetrix::Layer
{
private:
	blackbone::Process process;
	Icetrix::Console console;
	blackbone::Detour<f_wglSwapBuffers> d_wglSwapBuffers;

public:
	BOOL __stdcall h_wglSwapBuffers(HDC h);

	bool OnAttach();
	bool OnUpdate();
	bool OnDetach();

	void Draw();
	void ESP(playerent* pTargetPlayer, playerent* pLocalPlayer);
};