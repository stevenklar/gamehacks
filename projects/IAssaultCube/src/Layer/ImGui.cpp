#include "pch.h"
#include "ImGui.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>

bool Icetrix::ImGui::OnAttach()
{
    hwnd = CreateOverlay();
    ZeroMemory(&msg, sizeof(msg));

    return true;
}

bool Icetrix::ImGui::OnUpdate()
{
    enabled = (GetAsyncKeyState(VK_INSERT) & 1) ? !enabled : enabled;

    if (msg.message == WM_QUIT)
        return false;

    DispatchIncomingMessages(&msg);
	ImGui_ImplDX9_NewFrame();
    ChangeClickability(hwnd);
    Draw();
    Render();

    return true;
}

void Icetrix::ImGui::OnDetach()
{
    enabled = false;
    DestroyWindow(hwnd);
}

HWND Icetrix::ImGui::CreateOverlay()
{
	auto windowtitle = L"Moverleh";
    HWND hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, windowtitle, windowtitle, WS_POPUP, 1, 1, 1920, 1080, 0, 0, 0, 0);
    SetLayeredWindowAttributes(hwnd, 0, 1.0f, LWA_ALPHA);
    SetLayeredWindowAttributes(hwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);

    return hwnd;
}

void Icetrix::ImGui::DispatchIncomingMessages(LPMSG msg)
{
    if (PeekMessage(msg, NULL, 0U, 0U, PM_REMOVE))
	{
        TranslateMessage(msg);
        DispatchMessage(msg);
    }
}

void Icetrix::ImGui::ChangeClickability(HWND hwnd)
{
	long style = GetWindowLong(hwnd, GWL_EXSTYLE);
	if (enabled)
    {
		style &= ~WS_EX_LAYERED;
		SetWindowLong(hwnd, GWL_EXSTYLE, style);
        SetForegroundWindow(hwnd);
		windowstate = 1;
	}
	else
    {
		style |= WS_EX_LAYERED;
		SetWindowLong(hwnd, GWL_EXSTYLE, style);
		windowstate = 0;
	}
}

void Icetrix::ImGui::Draw()
{
	/*
    static float f = 0.0f;
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	*/
}

void Icetrix::ImGui::Render()
{
	/*
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, reinterpret_cast<const float*>(&clearColor));
    ImGui::Render();
    g_pSwapChain->Present(0, 0);
	*/
}