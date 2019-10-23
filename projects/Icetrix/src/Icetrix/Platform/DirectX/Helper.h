#pragma once

#include "d3d11.h"
#include "d3d9.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d9.lib")

/*
	Find DX11 SwapChain Manual Method

	// https://www.unknowncheats.me/wiki/IDA_Pro:Reverse_D3D11_Present_function_using_IDA_Pro
	f_Present present = reinterpret_cast<f_Present>(modules.GetModule(L"dxgi.dll")->baseAddress + 0x4300); // IDXGISwapChainPresent

	Find DX9 & DX11 SwapChain/Device/Context (auto)
	// https://guidedhacking.com/threads/get-direct3d9-and-direct3d11-devices-dummy-device-method.11867/
*/

namespace Icetrix::Platform::DirectX
{
	//void* SwapChain[18];
	//void* Device[40];
	//void* Context[108];
	bool GetD3D11SwapchainDeviceContext(void** pSwapchainTable, size_t Size_Swapchain, void** pDeviceTable, size_t Size_Device, void** pContextTable, size_t Size_Context);

	//void* d3d9Device[119];
	bool GetD3D9Device(HWND hwnd, void** pTable, size_t Size);
}
