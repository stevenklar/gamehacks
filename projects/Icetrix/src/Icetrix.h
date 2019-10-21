#pragma once

#include "pch.h"
#include "Icetrix/Application.h"
#include "Icetrix/Numerics.h"
#include "Icetrix/Process.h"
#include "Icetrix/Layer.h"
#include "Icetrix/Feature.h"
#include "Icetrix/Menu/ImGuiDesign.h"

#define LAYER(x) new x()

#define DUMP_OFFSET(x) std::cout << x << std::hex << &x << std::endl
#define LOG_INFO(x) std::cout << "[+] " << x << std::endl
#define LOG_ERROR(x) std::cout << "[!] " << x << std::endl

namespace Icetrix
{
	Application& CreateApplication();
}
