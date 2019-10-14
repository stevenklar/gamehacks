#pragma once

#include "Icetrix.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include <d3d11.h>

typedef void(__fastcall* f_Present)(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags);

class Visuals
{
private:
	blackbone::Detour<f_Present> d_Present;
public:
	Visuals(Icetrix::Application* app)
	{
		app->dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Visuals::OnAttach>(*this);
		app->dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&Visuals::OnDetach>(*this);
	}

	void OnAttach(const Icetrix::LayerEvent::Attach &attach);
	void OnDetach(const Icetrix::LayerEvent::Detach &detach);

	static void __fastcall h_Present(IDXGISwapChain*& pChain, UINT& SyncInterval, UINT& Flags);
	static LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
