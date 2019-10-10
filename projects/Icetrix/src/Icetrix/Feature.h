#pragma once
#include <vector>

namespace Icetrix
{
	class Feature
	{
	public:
		const char* label = "Unlabled";
		bool enabled = false;
	public:
		void Toggle();
	};

	class Features
	{
	private:
		Features() {};
		std::vector<Feature*>(featureMap);
	public:
		static Features* GetInstance();
		~Features();

		void Push(Feature* f);
		void Pop(Feature* f);

		Feature* Get(const char* label);
		std::vector<Feature*> Features::All();
		void Toggle(Feature* f);
	};
}
