#include "pch.h"
#include "PaperWalls.h"

void App::PaperWalls::Register()
{
	auto features = Icetrix::Features::GetInstance();
	features->Push(new Icetrix::Feature{"PaperWalls", false});
}

void App::PaperWalls::Update()
{
	if (Icetrix::Features::GetInstance()->Get("PaperWalls")->enabled)
		glDisable(GL_DEPTH_TEST);
}
