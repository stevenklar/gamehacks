#include <Windows.h>
#include "Process.h"
#include "BlackBone/Process/Process.h"

blackbone::Process* Icetrix::Process::GetIntance()
{
	static blackbone::Process* instance = new blackbone::Process;
	instance->Attach(GetCurrentProcessId());
	return instance;
}

Icetrix::Process::~Process()
{
	GetIntance()->Detach();
}
