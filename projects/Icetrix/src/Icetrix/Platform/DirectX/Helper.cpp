#include "pch.h"
#include "Helper.h"

bool Icetrix::Platform::DirectX::GetD3D11SwapchainDeviceContext(void** pSwapchainTable, size_t Size_Swapchain, void** pDeviceTable, size_t Size_Device, void** pContextTable, size_t Size_Context)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc{ 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetForegroundWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	IDXGISwapChain* pDummySwapChain = nullptr;
	ID3D11Device* pDummyDevice = nullptr;
	ID3D11DeviceContext* pDummyContext = nullptr;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pDummySwapChain, &pDummyDevice, NULL, &pDummyContext)))
	{
		swapChainDesc.Windowed = TRUE;
		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pDummySwapChain, &pDummyDevice, NULL, &pDummyContext)))
		{
			return false;
		}
	}

	if (pSwapchainTable)
	{
		memcpy(pSwapchainTable, *reinterpret_cast<void***>(pDummySwapChain), Size_Swapchain);
	}

	if (pDeviceTable)
	{
		memcpy(pDeviceTable, *reinterpret_cast<void***>(pDummyDevice), Size_Device);
	}

	if (pContextTable)
	{
		memcpy(pContextTable, *reinterpret_cast<void***>(pDummyContext), Size_Context);
	}

	pDummySwapChain->Release();
	pDummyDevice->Release();
	pDummyContext->Release();

	return true;
}

bool Icetrix::Platform::DirectX::GetD3D9Device(HWND hwnd, void** pTable, size_t Size)
{
	if (!pTable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = NULL;

	// options to create dummy device
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;

	HRESULT dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDeviceCreated != S_OK)
	{
		// may fail in windowed fullscreen mode, trying again with windowed mode
		d3dpp.Windowed = !d3dpp.Windowed;

		dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDeviceCreated != S_OK)
		{
			pD3D->Release();
			return false;
		}
	}

	memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), Size);

	pDummyDevice->Release();
	pD3D->Release();
	return true;
}
