#include "pch.h"
#include "Visuals.h"

#include "imgui.h"
#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_dx11.h"
#include "d3d11.h"

#pragma comment(lib, "d3d11.lib")

void __fastcall h_Present(IDXGISwapChain* &pChain, UINT &SyncInterval, UINT &Flags)
{
	static bool init = false;
	static ID3D11Device* g_pd3dDevice = NULL;
	static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	static ID3D11RenderTargetView* ppRTView = NULL;

	if (!init)
	{
		std::cout << "[+] Present::Init" << std::endl;
		HRESULT ret = pChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)&g_pd3dDevice);

		if (SUCCEEDED(ret))
			(*&g_pd3dDevice)->GetImmediateContext(&g_pd3dDeviceContext);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::StyleColorsDark();

		static HWND hwnd = NULL;

		while (hwnd == NULL)
			hwnd = FindWindowA(NULL, "Fallout4");

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

		ID3D11Texture2D* pBackBuffer;
		pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &ppRTView);
		pBackBuffer->Release();

		init = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	//Menu is displayed when g_ShowMenu is TRUE
	if (true)
	{
		bool bShow = true;
		ImGui::ShowDemoWindow(&bShow);
	}
	ImGui::EndFrame();

	ImGui::Render();

	g_pd3dDeviceContext->OMSetRenderTargets(1, &ppRTView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool Visuals::OnAttach()
{
	auto& modules = Icetrix::Process::GetInstance()->modules();	

	// https://www.unknowncheats.me/wiki/IDA_Pro:Reverse_D3D11_Present_function_using_IDA_Pro
	f_Present present = reinterpret_cast<f_Present>(modules.GetModule(L"dxgi.dll")->baseAddress + 0x4300); // IDXGISwapChainPresent
	
	if (d_Present.Hook(present, &h_Present, blackbone::HookType::HWBP))
	{
		std::cout << "[+] Hooked 'Present'" << std::endl;
	}
	else
	{
		std::cout << "[!] Failed to Hook: Present" << std::endl;
		return false;
	}

	return true;
}

bool Visuals::OnUpdate()
{
	return true;
}

void Visuals::OnDetach()
{
	d_Present.Restore();

	/*
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	*/
}

