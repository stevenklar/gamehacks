#pragma once

#include "Icetrix/Layer.h"
#include "Icetrix/Hook/DxgiSwapChainPresent.h"
#include "FW1FontWrapper.h"

class Watermark
{
private:
	Icetrix::Application& app;
	IDXGISwapChain* pChain = nullptr;
	inline static ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	IFW1FontWrapper* pFontWrapper = NULL;
public:
	Watermark() : app(Icetrix::Application::GetInstance())
	{
		app.dispatcher.sink<Icetrix::Hook::PresentEvent::Initialize>().connect<&Watermark::Init>(*this);
		app.dispatcher.sink<Icetrix::Hook::PresentEvent::Update>().connect<&Watermark::Render>(*this);
	}

	void Init(const Icetrix::Hook::PresentEvent::Initialize& init)
	{
		pChain = init.pChain;
		HRESULT ret = pChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)&g_pd3dDevice);

		if (SUCCEEDED(ret))
			(*&g_pd3dDevice)->GetImmediateContext(&g_pd3dDeviceContext);

		IFW1Factory* pFW1Factory;
		FW1CreateFactory(0x110f, &pFW1Factory);

		if (g_pd3dDevice)
		{
			pFW1Factory->CreateFontWrapper(g_pd3dDevice, L"Verdana", &pFontWrapper);
			pFW1Factory->Release();
		}
	}

	void Render()
	{
		ID3D11Texture2D* pRenderTargetTexture;

		if (SUCCEEDED(pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pRenderTargetTexture)))
		{
			ID3D11RenderTargetView* g_pRenderTargetView = NULL;
			g_pd3dDevice->CreateRenderTargetView(pRenderTargetTexture, NULL, &g_pRenderTargetView);
			g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
			pFontWrapper->DrawString(g_pd3dDeviceContext, L"metrix from guidehacking.com", 15.0f, 10.0f, 10.0f, 0xFFFFFF00, FW1_RESTORESTATE);
			pRenderTargetTexture->Release();
		}
	}
};