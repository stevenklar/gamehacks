#pragma once

#include "Icetrix.h"
#include "BlackBone/LocalHook/LocalHook.hpp"

class Visuals
{
private:
	Icetrix::Application& app;
public:
	Visuals() : app(Icetrix::Application::GetInstance())
	{
		app.dispatcher.sink<Icetrix::Hook::PresentEvent::Initialize>().connect<&Visuals::InitializePresent>(*this);
		app.dispatcher.sink<Icetrix::Hook::PresentEvent::Update>().connect<&Visuals::Update>(*this);
		app.dispatcher.sink<Icetrix::Hook::PresentEvent::Unhook>().connect<&Visuals::Shutdown>(*this);
	}

	void InitializePresent(const Icetrix::Hook::PresentEvent::Initialize &init);
	void Update();
	void Shutdown();

	static LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	inline static WNDPROC WndProcHandler = NULL;
	inline static HWND hwnd = NULL;

	inline static bool bShow = false;
	LONG_PTR OriginalWndProc = NULL;

	inline static ID3D11Device* g_pd3dDevice = NULL;
	ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	ID3D11RenderTargetView* ppRTView = NULL;
};
