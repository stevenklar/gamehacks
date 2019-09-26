//
// DirectX 9 Wrapper
//
#include "pch.h"
#include "DxWrapper.h"

bool GetD3D9Device(void** pTable, size_t Size)
{
	if (!pTable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
		return false;

	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetForegroundWindow();
	d3dpp.Windowed = ((GetWindowLong(d3dpp.hDeviceWindow, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;;

	IDirect3DDevice9* pDummyDevice = nullptr;
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice)))
	{
		pD3D->Release();
		return false;
	}

	memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), Size);

	pDummyDevice->Release();
	pD3D->Release();

	return true;
}

void Draw(LPCSTR text, RECT rect, D3DCOLOR color, LPD3DXFONT& font)
{
	font->DrawTextA(NULL, text, strlen(text), &rect, DT_NOCLIP, color);
}

LPD3DXFONT font;
LPD3DXFONT fontBold;

D3DCOLOR WhiteColor = D3DCOLOR_RGBA(255, 255, 255, 255);
D3DCOLOR RedColor = D3DCOLOR_RGBA(255, 0, 0, 255);
D3DCOLOR GreenColor = D3DCOLOR_RGBA(0, 255, 0, 255);
D3DCOLOR BlueColor = D3DCOLOR_RGBA(0, 0, 255, 255);
D3DCOLOR BlackColor = D3DCOLOR_RGBA(0, 0, 0, 255);

typedef HRESULT(__stdcall* f_EndScene)(IDirect3DDevice9* pDevice);
f_EndScene o_EndScene;
// Detour<f_EndScene> d_EndScene;
bool fontCreated = false;
bool showMenu = true;

HRESULT __stdcall h_EndScene(IDirect3DDevice9* pDevice)
{
	if (!fontCreated)
	{
		D3DXCreateFontA(pDevice, 16, 6, FW_BOLD, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &font);
		D3DXCreateFontA(pDevice, 16, 6, FW_BOLD, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &fontBold);
		fontCreated = true;
	}

	if (fontCreated && showMenu)
	{
		Draw("Hack by [metrix] feat. GuidedHacking.com", { 10, 10, 10, 10 }, WhiteColor, fontBold);
		Draw("Hack by [metrix] feat. GuidedHacking.com", { 20, 20, 20, 20 }, WhiteColor, fontBold);
		Draw("Hack by [metrix] feat. GuidedHacking.com", { 30, 30, 30, 30 }, WhiteColor, fontBold);
		Draw("Hack by [metrix] feat. GuidedHacking.com", { 40, 40, 40, 40 }, WhiteColor, fontBold);
		std::cout << "[h_wglSwapBuffers]" << std::endl;
	}

	return o_EndScene(pDevice);
}
