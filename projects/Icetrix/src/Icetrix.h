#pragma once

#include "pch.h"
#include "Icetrix/Application.h"
#include "Icetrix/Numerics.h"
#include "Icetrix/Process.h"
#include "Icetrix/Layer.h"
#include "Icetrix/Feature.h"
#include "Icetrix/Menu/ImGuiDesign.h"

#include "Icetrix/Layer/PanicKey.h"
#include "Icetrix/Hook/GlBindTexture.h"
#include "Icetrix/Hook/SdlPollEvent.h"
#include "Icetrix/Hook/WglSwapBuffers.h"

namespace Icetrix
{
	Application& CreateApplication();
}
