#pragma once
#include "pch.h"
#include "Icetrix/Process.h"

#define MAX_PATCH 12

namespace Icetrix::Memory
{
    struct Patch
    {
        char const* name;
        uint64_t address;
        BYTE original[MAX_PATCH];
        BYTE patch[MAX_PATCH];
        int size;
    };

    class BytePatch
    {
    public:
        static void Patch(Icetrix::Memory::Patch patch, uint64_t baseAddress = 0);
        static void Unpatch(Icetrix::Memory::Patch patch, uint64_t baseAddress = 0);
    };
}