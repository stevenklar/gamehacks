#pragma once

#include <windows.h>
#include <cstdint>
#include <tlhelp32.h>
#include <Psapi.h>
#include "Icetrix/Layer.h"
#include <iostream>

class NoRecoil : public Icetrix::Layer
{
public:
	bool OnAttach();
	bool OnUpdate();
	void OnDetach();
};

