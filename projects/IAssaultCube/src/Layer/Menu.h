#pragma once
#include "Icetrix/Layer.h"

class Menu : public Icetrix::Layer
{
public:
	bool OnAttach();
	bool OnUpdate();
	void OnDetach();
};