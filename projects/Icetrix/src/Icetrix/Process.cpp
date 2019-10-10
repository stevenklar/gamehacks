#include "pch.h"
#include "Process.h"
#include "BlackBone/Process/Process.h"

blackbone::Process* Icetrix::Process::GetInstance()
{
	static blackbone::Process* instance = new blackbone::Process;
	instance->Attach(GetCurrentProcessId());
	return instance;
}

Icetrix::Process::~Process()
{
	GetInstance()->Detach();
	delete GetInstance();
}
