#include "pch.h"
#include "Feature.h"

void Icetrix::Feature::Toggle()
{
	enabled = !enabled;
}

Icetrix::Features* Icetrix::Features::GetInstance()
{
	static Features* instance = new Icetrix::Features;
	return instance;
}

Icetrix::Features::~Features()
{
	for (auto f : featureMap)
		delete f;
}

void Icetrix::Features::Push(Icetrix::Feature* feature)
{
	featureMap.push_back(feature);
}

void Icetrix::Features::Pop(Icetrix::Feature* feature)
{
	// implement deletion of features
}

Icetrix::Feature* Icetrix::Features::Get(const char* label)
{
	for (auto f : featureMap)
	{
		if (label == f->label)
			return f;
	}

	return NULL;
}

std::vector<Icetrix::Feature*> Icetrix::Features::All()
{
	return featureMap;
}

void Icetrix::Features::Toggle(Icetrix::Feature* feature)
{
	for (auto f : featureMap)
	{
		if (feature == f)
			f->enabled = !f->enabled;
	}
}
