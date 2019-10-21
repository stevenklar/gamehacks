#pragma once

#include <Windows.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <entt.hpp>

#include "Icetrix/Application.h"

#define LAYER(x) new x()

#define DUMP_OFFSET(x) std::cout << x << std::hex << &x << std::endl
#define LOG_INFO(x) std::cout << "[+] " << x << std::endl
#define LOG_ERROR(x) std::cout << "[!] " << x << std::endl