#include "pch.h"
#include "GlBindTexture.h"

void Hook::GlBindTexture::Hook(const Icetrix::LayerEvent::Attach& attach)
{
	HMODULE hModule = GetModuleHandle("opengl32.dll");

	if (hModule == NULL)
	{
		LOG_ERROR("Failed to find opengl32.dll");
		return;
	}

	auto dglBindTexture = GetProcAddress(hModule, "glBindTexture");
	f_glBindTexture glBindTexture = reinterpret_cast<f_glBindTexture>(dglBindTexture);

	if (d_glBindTexture.Hook(glBindTexture, &GlBindTexture::h_glBindTexture, blackbone::HookType::HWBP))
		LOG_INFO("Hooked 'glBindTexture'");
	else
		LOG_ERROR("Failed to Hook: glBindTexture");
}

void Hook::GlBindTexture::Unhook(const Icetrix::LayerEvent::Detach& detach)
{
	if (d_glBindTexture.Restore())
		LOG_INFO("Restored 'glBindTexture'");
	else
		LOG_ERROR("Failed to restore glBindTexture");
}

void __stdcall Hook::GlBindTexture::h_glBindTexture(GLenum& target, GLuint& texture)
{
	auto& app = Icetrix::Application::GetInstance();
	static bool initialized = false;

	if (!initialized)
	{
		LOG_INFO("GlBindTexture: Initialized");
		app.dispatcher.trigger<GlBindTextureEvent::Initialize>();
		initialized = true;
	}

	app.dispatcher.trigger<GlBindTextureEvent::Update>();
}
