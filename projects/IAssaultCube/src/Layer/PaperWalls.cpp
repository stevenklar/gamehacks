#include "pch.h"
#include "PaperWalls.h"

void App::PaperWalls::RegisterFeature(const Hook::GlBindTextureEvent::Initialize& attach)
{
	auto features = Icetrix::Features::GetInstance();
	features->Push(new Icetrix::Feature{"PaperWalls", false});
}

void App::PaperWalls::Update(const Hook::GlBindTextureEvent::Update& update)
{
	if (Icetrix::Features::GetInstance()->Get("PaperWalls")->enabled)
		glDisable(GL_DEPTH_TEST);
}
