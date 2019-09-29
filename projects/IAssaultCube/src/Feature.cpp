#include "Feature.h"

Features* Features::GetInstance()
{
	static Features* instance = new Features;
	return instance;
}

Features::~Features()
{
	for (auto f : featureMap)
		delete f;
}

void Features::Push(Feature* feature)
{
	featureMap.push_back(feature);
}

void Features::Pop(Feature* feature)
{
	// implement deletion of features
}

Feature* Features::Get(const char* label)
{
	for (auto f : featureMap)
	{
		if (label == f->label)
			return f;
	}

	return NULL;
}

std::vector<Feature*> Features::All()
{
	return featureMap;
}

void Features::Toggle(Feature* feature)
{
	for (auto f : featureMap)
	{
		if (feature == f)
			f->enabled = !f->enabled;
	}
}

