-- Workspace
workspace "GameHacks"
    location (BUILD_DIR .. "gen")
    targetdir(BUILD_DIR .. "bin/%{prj.name}/%{cfg.longname}")
    objdir   (BUILD_DIR .. "obj/%{prj.name}/%{cfg.longname}")
    language   "C++"
    cppdialect "C++latest"

    configurations { "Debug", "Release" }
    platforms      { "x86", "x64" }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "WINDOWS_IGNORE_PACKING_MISMATCH",
        "NOMINMAX",
    }

    flags "MultiProcessorCompile"

    filter "platforms:x86"
        architecture "x86"
        defines "IX_WIN32"

    filter "platforms:x64"
        architecture "x64"
        defines "IX_WIN64"

    filter "configurations:Debug"
        staticruntime "On"
        symbols "On"

    filter "configurations:Release"
        staticruntime "On"
        symbols "Off"
        optimize "On"

    filter {}