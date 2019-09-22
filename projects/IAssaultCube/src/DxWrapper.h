#pragma once

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#include <d3d9.h>
#include <iostream>

bool GetD3D9Device(void** pTable, size_t Size);
void Draw(LPCSTR text, RECT rect, D3DCOLOR color, LPD3DXFONT& font);

// DirectX
/*
std::cout << "[+] Search for D3D9Device" << std::endl;
void* d3d9Device[119];
if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
{
	std::cout << "[+] Found DirectXDevice vTable at: ";
	std::cout << std::hex << d3d9Device[0] << "\n";

	std::cout << "[+] Trying to hook function..\n";
	//o_Reset = (f_Reset)reset[16];
	o_EndScene = (f_EndScene)d3d9Device[42];

	//typedef HRESULT(__stdcall* f_EndScene)(IDirect3DDevice9* pDevice);

	// Hook endScene to draw in directx9
	if (d_EndScene.Hook((long(__stdcall*)(IDirect3DDevice9*))o_EndScene, (long(__stdcall*)(IDirect3DDevice9*&))h_EndScene, HookType::Int3))
	{
		std::cout << "[+] Hooked EndScene\n";
	}
	else
	{
		std::cout << "[!] Failed to Hook: EndScene\n";
	}
}
else
{
	std::cout << "[!] Search for D3D9Device failed" << std::endl;
}
*/

