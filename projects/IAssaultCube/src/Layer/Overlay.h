#pragma once
#include "Icetrix/Layer.h"
#include <Windows.h>
#include <d3dx9.h>

void initD3D(HWND hWnd);
void DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char* fmt);
void render();
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Overlay : public Icetrix::Layer
{
public:
	bool OnAttach();
	bool OnUpdate();
	void OnDetach();
};
