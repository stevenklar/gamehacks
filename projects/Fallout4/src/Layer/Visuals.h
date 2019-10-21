#pragma once

#include "Icetrix.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include <d3d11.h>

typedef void(__fastcall* f_Present)(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags);

class Visuals
{
private:
	blackbone::Detour<f_Present> d_Present;
	Icetrix::Application& app;
public:
	Visuals() : app(Icetrix::Application::GetInstance())
	{
		app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&Visuals::OnAttach>(*this);
		app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&Visuals::OnDetach>(*this);
	}

	void OnAttach();
	void OnDetach();

	static void __fastcall h_Present(IDXGISwapChain*& pChain, UINT& SyncInterval, UINT& Flags);
	static LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
