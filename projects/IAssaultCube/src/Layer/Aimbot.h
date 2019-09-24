#pragma once

#include "Icetrix/Layer.h"

class Aimbot : public Icetrix::Layer
{
public:
	bool OnAttach()
	{
		return true;
	}

	bool OnUpdate()
	{
		return true;
	}

	void OnDetach()
	{
	}
};
