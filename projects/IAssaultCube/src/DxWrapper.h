#pragma once

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#include <d3d9.h>
#include <iostream>

bool GetD3D9Device(void** pTable, size_t Size);
void Draw(LPCSTR text, RECT rect, D3DCOLOR color, LPD3DXFONT& font);
