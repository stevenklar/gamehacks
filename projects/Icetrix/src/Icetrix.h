#pragma once

#include "pch.h"
#include "Icetrix/Application.h"
#include "Icetrix/Numerics.h"
#include "Icetrix/Process.h"
#include "Icetrix/Layer.h"
#include "Icetrix/Feature.h"

//#define LAYER(x) std::make_shared<x>(this)
#define LAYER(x) new x(this)

namespace Icetrix
{
	Application* CreateApplication();
}
