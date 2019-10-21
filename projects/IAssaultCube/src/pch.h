#pragma once

// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include <Windows.h>

// Std Header Files
#include <iostream>
#include <array>
#include <vector>
#include <memory>

// Custom Header Files
#include "Icetrix.h"
#include "Icetrix/Platform/OpenGL/OpenGL.h"
#include "Game/Game.h"

// Application Layers
#include "Layer/Menu.h"
#include "Layer/ESP.h"
#include "Layer/Patches.h"
#include "Layer/PaperWalls.h"