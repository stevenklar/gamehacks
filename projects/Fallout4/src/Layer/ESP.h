#pragma once

#include "Icetrix/Layer.h"

class ESP
{
private:
	Icetrix::Application& app;
public:
	ESP() : app(Icetrix::Application::GetInstance())
	{
		// get entites / list
		// iterate over entities
		// mycords + entity cords => world 2 screen
		// draw result
		// profit

		// player = 0x7FF744A33858
		// other  = 0x7FF744A2F328
		
		// (1) hook -> entity collector
		// (2) invalidate entities
		// (3) draw entities

		//Fallout4.exe+2F7A74
		// \x48\x8b\x06\x48\x8b\xce\xff\x90\x00\x00\x00\x00\x48\x8b\x88\x00\x00\x00\x00\xe8 xxxxxxxx????xxx????x
	}


};