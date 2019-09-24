#pragma once
#include "Layer.h"
#include <Windows.h>

namespace Icetrix
{
    class ImGui : public Layer
    {
    private:
        int windowstate = 1;
        bool enabled = true;
        MSG msg;
        HWND hwnd;
    
    private:
        HWND CreateWindow();
        void DispatchIncomingMessages(LPMSG msg);
        void ChangeClickability(bool canclick, HWND ownd);
        void Render();
        void Draw();

    public:
        bool OnAttach();
        bool OnUpdate();
        void OnDetach();
    };
}