#pragma once

#include "BlackBone/LocalHook/LocalHook.hpp"
#include <dxgi.h>
#include <3rd_party\AsmJit\x86\x86operand.h>

namespace App
{
	typedef void(* f_decreaseAmmo)();

	class GetEntityHook
	{
	private:
		Icetrix::Application& app;
		blackbone::Process* process;

		int decreaseAmmoAddress = 0x4637E1;
		static const int needBytes = 10;
		int AmmoAdress = 0;

		int checkGrenadeAddress = 0x408C41;
		int GrenadeAddress = 0;

		blackbone::AsmHelperPtr aHook = blackbone::AsmFactory::GetAssembler();
		blackbone::AsmHelperPtr aOriginal = blackbone::AsmFactory::GetAssembler();
		blackbone::IAsmHelper& h = *aHook;
		blackbone::IAsmHelper& o = *aOriginal;

		blackbone::AsmHelperPtr gHook = blackbone::AsmFactory::GetAssembler();
		blackbone::AsmHelperPtr gOriginal = blackbone::AsmFactory::GetAssembler();
		blackbone::IAsmHelper& hg = *gHook;
		blackbone::IAsmHelper& og = *gOriginal;
	public:
		blackbone::call_result_t<blackbone::MemBlock> hookResult;
		blackbone::call_result_t<blackbone::MemBlock> grenadeHookResult;

		GetEntityHook() : app(Icetrix::Application::GetInstance()), process(Icetrix::Process::GetInstance())
		{
			app.dispatcher.sink<Icetrix::LayerEvent::Attach>().connect<&GetEntityHook::Attach>(*this);
			app.dispatcher.sink<Icetrix::LayerEvent::Update>().connect<&GetEntityHook::Update>(*this);
			app.dispatcher.sink<Icetrix::LayerEvent::Detach>().connect<&GetEntityHook::Detach>(*this);
		}

		void Attach()
		{
			HookDecreaseAmmo();
			HookCheckGrenade();
		}

		void HookDecreaseAmmo()
		{
			// hook prolog
			h->pop(h->zax);
			// the actual hook - copied existing code
			h->mov(h->zdx, asmjit::host::ptr(h->zsi, 0x18));
			h->mov(asmjit::host::ptr(h->zdx), h->zcx);
			h->mov(h->zsi, asmjit::host::ptr(h->zsi, 0x14));
			
			// get register content into variable
			h->push(h->zax);
			h->mov(h->zax, (unsigned)&AmmoAdress);
			h->mov(asmjit::host::ptr(h->zax), h->zsi);
			h->pop(h->zax);

			// hook epilog
			h->push(h->zbx);
			h->mov(h->zbx, decreaseAmmoAddress + needBytes - 2);
			h->jmp(h->zbx);

			hookResult = process->memory().Allocate(h->getCodeSize(), PAGE_EXECUTE_READWRITE, process->modules().GetMainModule()->baseAddress);

			// Jump to hook
			o->push(o->zax);
			o->mov(o->zax, hookResult->ptr());
			o->jmp(o->zax);
			o->pop(o->zbx);
			o->nop();

			if (NT_SUCCESS(process->memory().Protect(hookResult->ptr(), h->getCodeSize(), PAGE_EXECUTE_READWRITE)))
				hookResult->Write(0, h->getCodeSize(), h->make());

			if (NT_SUCCESS(process->memory().Protect(decreaseAmmoAddress, o->getCodeSize(), PAGE_EXECUTE_READWRITE)))
				process->memory().Write(decreaseAmmoAddress, o->getCodeSize(), o->make());

			LOG_INFO("Inline Hooked: AC_DecreaseAmmo");
		}

		void HookCheckGrenade()
		{
			auto& h = hg;
			auto& o = og;

			// hook prolog
			h->add(h->zsp, 0x0C);
			h->cmp(asmjit::host::dword_ptr(h->zsi, 0x158), 0x0);
			h->pushf();

			h->mov(h->zax, (unsigned)&GrenadeAddress);
			h->add(h->zsi, 0x158);
			h->mov(asmjit::host::ptr(h->zax), h->zsi);
			h->sub(h->zsi, 0x158);
			h->pop(h->zax);

			h->push(h->zbx);
			h->mov(h->zbx, checkGrenadeAddress + needBytes - 2);
			h->jmp(h->zbx);

			grenadeHookResult = process->memory().Allocate(h->getCodeSize(), PAGE_EXECUTE_READWRITE, process->modules().GetMainModule()->baseAddress);

			// Jump to hook
			o->push(o->zax);
			o->mov(o->zax, grenadeHookResult->ptr());
			o->jmp(o->zax);
			o->pop(o->zbx);
			o->popf();

			if (NT_SUCCESS(process->memory().Protect(grenadeHookResult->ptr(), h->getCodeSize(), PAGE_EXECUTE_READWRITE)))
				grenadeHookResult->Write(0, h->getCodeSize(), h->make());

			if (NT_SUCCESS(process->memory().Protect(checkGrenadeAddress, o->getCodeSize(), PAGE_EXECUTE_READWRITE)))
				process->memory().Write(checkGrenadeAddress, o->getCodeSize(), o->make());

			LOG_INFO("Inline Hooked: AC_CheckGrenadeCount");
		}

		void Update()
		{
			entt::monostate<"ammoAddress"_hs>{} = AmmoAdress;
			entt::monostate<"grenadeAddress"_hs>{} = GrenadeAddress;
		}

		void Detach()
		{
			auto process = Icetrix::Process::GetInstance();

			process->memory().Write(decreaseAmmoAddress, std::array<BYTE, needBytes>{0x8B, 0x56, 0x18, 0x89, 0x0A, 0x8B, 0x76, 0x14, 0xFF, 0x0E});
			hookResult->Free();
			LOG_INFO("Unhooked: AC_DecreaseAmmo");

			process->memory().Write(checkGrenadeAddress, std::array<BYTE, needBytes>{0x83, 0xC4, 0x0C, 0x83, 0xBE, 0x58, 0x01, 0x00, 0x00, 0x00});
			grenadeHookResult->Free();
			LOG_INFO("Unhooked: AC_CheckGrenadeCount");
		}
	};
}
