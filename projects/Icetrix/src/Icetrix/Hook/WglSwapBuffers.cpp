#include "pch.h"
#include "WglSwapBuffers.h"

void Icetrix::Hook::WglSwapBuffers::Hook(const Icetrix::LayerEvent::Attach& attach)
{
	HMODULE hOpenglModule = GetModuleHandle("opengl32.dll");

	if (hOpenglModule == NULL)
	{
		LOG_ERROR("Failed to find opengl32.dll::wglSwapBuffers");
		return;
	}

	auto dWglSwapBuffers = GetProcAddress(hOpenglModule, "wglSwapBuffers");
	f_wglSwapBuffers wglSwapBuffers = reinterpret_cast<f_wglSwapBuffers>(dWglSwapBuffers);

	if (d_wglSwapBuffers.Hook(wglSwapBuffers, &Hook::WglSwapBuffers::h_wglSwapBuffers, blackbone::HookType::HWBP))
	{
		LOG_INFO("Hooked 'wglSwapBuffers'");
		return;
	}
	else
	{
		LOG_ERROR("Failed to Hook: wglSwapBuffers");
		return;
	}
}

void Icetrix::Hook::WglSwapBuffers::Unhook(const Icetrix::LayerEvent::Detach& detach)
{
	if (d_wglSwapBuffers.Restore())
		LOG_INFO("Restored 'wglSwapBuffers'");
	else
		LOG_ERROR("Failed to restore wglSwapBuffers");
}

void __stdcall Icetrix::Hook::WglSwapBuffers::h_wglSwapBuffers(HDC& h)
{
	auto& app = Icetrix::Application::GetInstance();
	static bool initialized = false;

	if (!initialized)
	{
		LOG_INFO("h_wglSwapBuffers: Initialized");
		app.dispatcher.trigger<WglSwapBufferEvent::Initialize>();
		initialized = true;
	}

	app.dispatcher.trigger<WglSwapBufferEvent::Update>();
}	

