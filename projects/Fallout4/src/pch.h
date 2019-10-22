#pragma once

// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include <Windows.h>

// Std Header Files
#include <iostream>
#include <array>
#include <cstdint>
#include <vector>
#include <tlhelp32.h>
#include <Psapi.h>

// Custom Header Files
#include <Icetrix.h>

// Application Layers
#include "Layer/Patches.h"
#include "Layer/Visuals.h"
#include "Layer/Godmode.h"
#include "Layer/Weapon.h"
