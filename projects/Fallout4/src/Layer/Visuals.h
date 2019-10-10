#pragma once

#include "Icetrix.h"
#include "BlackBone/LocalHook/LocalHook.hpp"
#include <d3d11.h>

typedef void(__fastcall* f_Present)(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags);

class Visuals : public Icetrix::Layer
{
private:
	blackbone::Detour<f_Present> d_Present;
public:
	bool OnAttach();
	bool OnUpdate();
	void OnDetach();
};
