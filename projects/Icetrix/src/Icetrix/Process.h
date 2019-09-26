#pragma once

#include "BlackBone/Process/Process.h"

namespace Icetrix
{
	class Process
	{
	private:
		Process();
		Process(const Process&);
	public:
		static blackbone::Process* GetIntance();
		~Process();
	};
}