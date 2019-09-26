#pragma once
#include "Icetrix/Layer.h"
#include <Windows.h>

namespace Icetrix
{
    class ImGui : public Icetrix::Layer
    {
    private:
        int windowstate = 1;
        bool enabled = true;
        MSG msg;
        HWND hwnd;
    
    private:
        HWND CreateOverlay();
        void DispatchIncomingMessages(LPMSG msg);
        void ChangeClickability(HWND hwnd);
        void Render();
        void Draw();

    public:
        bool OnAttach();
        bool OnUpdate();
        void OnDetach();
    };
}