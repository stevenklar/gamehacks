#include "pch.h"
#include "DxgiSwapChainPresent.h"
#include "Icetrix/Process.h"
#include "Icetrix/Platform/DirectX/Helper.h"

void Icetrix::Hook::DxgiSwapChainPresent::Hook(const Icetrix::LayerEvent::Attach& attach)
{
	auto& modules = Icetrix::Process::GetInstance()->modules();
	f_Present present = reinterpret_cast<f_Present>(modules.GetModule(L"dxgi.dll")->baseAddress + presentOffset); // IDXGISwapChainPresent
	
	if (d_Present.Hook(present, &Icetrix::Hook::DxgiSwapChainPresent::h_Present, blackbone::HookType::HWBP))
		LOG_INFO("Hooked 'IDXGISwapChain::Present'");
	else
		LOG_ERROR("Failed to Hook: IDXGISwapChain::Present");
}

void Icetrix::Hook::DxgiSwapChainPresent::Unhook(const Icetrix::LayerEvent::Detach& detach)
{
	auto& app = Icetrix::Application::GetInstance();

	if (d_Present.Restore())
		LOG_INFO("Restored 'IDXGISwapChain::Present'");
	else
		LOG_ERROR("Failed to restore IDXGISwapChain::Present");

	app.dispatcher.trigger<PresentEvent::Unhook>();
}

void __fastcall Icetrix::Hook::DxgiSwapChainPresent::h_Present(IDXGISwapChain* &pChain, UINT &SyncInterval, UINT &Flags)
{
	auto& app = Icetrix::Application::GetInstance();
	static bool initialized = false;

	app.dispatcher.trigger<PresentEvent::PreUpdate>();

	if (!initialized)
	{
		LOG_INFO("IDXGISwapChain::Present: Initialized");
		app.dispatcher.trigger<PresentEvent::Initialize>(pChain);
		initialized = true;
	}

	app.dispatcher.trigger<PresentEvent::Update>();
}
