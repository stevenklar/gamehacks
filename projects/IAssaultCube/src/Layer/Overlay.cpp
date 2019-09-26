#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <dwmapi.h>
#include <charconv>
#include <TlHelp32.h>
#include "Game/Game.h"
#include "Overlay.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

// Dx9
const char* windowName = "AssaultCube";
int width = 800, height = 600;
const MARGINS  margin = { 0, 0, width, height };
IDirect3D9* iDirect3d9;
IDirect3DDevice9* dDirect3d9;
HWND hWnd;
LPD3DXFONT pFont;
MSG msg;

// ImGui
bool show_demo_window = true;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool Overlay::OnUpdate()
{
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	if (!FindWindow(NULL, windowName))
		ExitProcess(0);

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// -- ImGui --
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::EndFrame();

	render();
	// -- ImGui --/


	if (msg.message == WM_QUIT)
	{
		std::cout << "[!] Overlay :: exit!";
		//return false;
	}

	return true;
}

void Overlay::OnDetach()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow(hWnd);
}

void initD3D(HWND hWnd)
{
	// gather screen params
	height = ScreenSettings::GetInstance()->m_Height;
	width = ScreenSettings::GetInstance()->m_Width;

	// create interface
	iDirect3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	
	// define window params
	D3DPRESENT_PARAMETERS params;
	ZeroMemory(&params, sizeof(params));
	params.Windowed = true;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = hWnd;
	params.BackBufferFormat = D3DFMT_X8R8G8B8;
	params.BackBufferWidth = width;
	params.BackBufferHeight = height;
	params.EnableAutoDepthStencil = true;
	params.AutoDepthStencilFormat = D3DFMT_D16;

	// create device and font
	iDirect3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &dDirect3d9);
	D3DXCreateFont(dDirect3d9, 50, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &pFont);

}

void DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char* fmt)
{
	RECT FontPos = { x, y, x + 120, y + 16 };
	char buf[1024] = { '\0' };
	va_list va_alist;

	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);
	g_pFont->DrawText(NULL, buf, -1, &FontPos, DT_NOCLIP, color);
}

void render()
{
	// clear the window alpha
	dDirect3d9->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	dDirect3d9->BeginScene();    // begins the 3D scene

	/*
	Game game;
	int numOfPlayers = game.GetNumberOfPlayers();
	char text[5] = {};
	std::sprintf(text, "P: %d", numOfPlayers);
	DrawString(10, 50, D3DCOLOR_ARGB(255, 255, 0, 0), pFont, text);
	*/

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	dDirect3d9->EndScene();    // ends the 3D scene
	dDirect3d9->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_PAINT:
	{
		DwmExtendFrameIntoClientArea(hWnd, &margin);
	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Overlay::OnAttach()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	RECT rc;
	HWND newhwnd = FindWindow(NULL, "AssaultCube");
	if (newhwnd != NULL) {
		GetWindowRect(newhwnd, &rc);
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
	}
	else {
		ExitProcess(0);
	}
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(0,
		"WindowClass",
		"",
		WS_EX_TOPMOST | WS_POPUP,
		rc.left, rc.top,
		width, height,
		NULL,
		NULL,
		hInstance,
		NULL);

	SetWindowLong(hWnd, GWL_EXSTYLE, (int)GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);

	ShowWindow(hWnd, SW_SHOW);

	initD3D(hWnd);
	::SetWindowPos(FindWindow(NULL, windowName), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(dDirect3d9);

	//return msg.wParam;
	return true;
}
