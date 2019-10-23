#pragma once
#include <BlackBone/LocalHook/LocalHook.hpp>
#include <d3d11.h>
#include "Icetrix/Application.h"

typedef void(__fastcall* f_Present)(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags);

namespace Icetrix::Hook
{
	namespace PresentEvent
	{
		struct PreUpdate {};
		struct Initialize { IDXGISwapChain*& pChain; };
		struct Update {};
		struct Unhook {};
	}

	class DxgiSwapChainPresent
	{
	private:
		Icetrix::Application& app;
		blackbone::Detour<f_Present> d_Present;
		DWORD presentOffset;
	public:
		DxgiSwapChainPresent(DWORD presentOffset) : app(Icetrix::Application::GetInstance())
		{
			this->presentOffset = presentOffset;
			app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&DxgiSwapChainPresent::Hook>(*this);
			app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&DxgiSwapChainPresent::Unhook>(*this);
		}

		void Hook(const Icetrix::LayerEvent::Attach &attach);
		void Unhook(const Icetrix::LayerEvent::Detach &detach);

		static void __fastcall h_Present(IDXGISwapChain*& pChain, UINT& SyncInterval, UINT& Flags);
	};
}
