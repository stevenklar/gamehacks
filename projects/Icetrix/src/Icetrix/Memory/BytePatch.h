#pragma once
#include "pch.h"
#include "Icetrix/Process.h"

namespace Icetrix::Memory
{
    struct Patch
    {
        char const* name;
        uint64_t address;
		std::vector<BYTE> original;
		std::vector<BYTE> patch;
    };

    class BytePatch
    {
    public:
        static void Patch(Icetrix::Memory::Patch patch, uint64_t baseAddress = 0);
        static void Unpatch(Icetrix::Memory::Patch patch, uint64_t baseAddress = 0);
    };
}