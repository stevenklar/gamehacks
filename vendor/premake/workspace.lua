-- Workspace
workspace "GameHacks"
    location (BUILD_DIR .. "gen")
    targetdir(BUILD_DIR .. "bin/%{prj.name}/%{cfg.longname}")
    objdir   (BUILD_DIR .. "obj/%{prj.name}/%{cfg.longname}")
    language   "C++"
    cppdialect "C++latest"

    configurations { "Debug", "Release" }
    platforms      { "Win32", "Win64" }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "WINDOWS_IGNORE_PACKING_MISMATCH",
        "NOMINMAX",
    }

    flags {
        "MultiProcessorCompile",    -- Enable Visual Studio to use multiple compiler processes when building.
    }

    filter "platforms:Win32"
        architecture "x86"
        defines "IX_WIN32"

    filter "platforms:x64"
        architecture "x64"
        defines "IX_WIN64"

    filter "configurations:Debug"
        staticruntime "On"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        staticruntime "On"
        symbols "Off"
        optimize "Full"

    filter {}