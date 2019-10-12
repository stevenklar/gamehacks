-- Prepare some dynamic variables
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Workspace
workspace "GameHacks"
    location ("build/gen")
    targetdir("build/bin/%{prj.name}/%{cfg.longname}")
    objdir   ("build/obj/%{prj.name}/%{cfg.longname}")

    defines { "_CRT_SECURE_NO_WARNINGS" }

    language       "C++"
    cppdialect     "C++latest"

    configurations { "Debug", "Release" }
    platforms      { "Win32", "Win64" }

    filter "platforms:Win32"
        architecture "x86"
        defines "IX_WIN32"
    filter "platforms:Win64"
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

    flags {
        "MultiProcessorCompile"
    }
    defines {
        "WINDOWS_IGNORE_PACKING_MISMATCH"
    }

-- Helper
function includeProject()
    files {
        "projects/%{prj.name}/src/**",
    }

    includedirs "projects/%{prj.name}/src"
    includedirs "projects/%{prj.name}/vendor"
end

function includeDirectX()
    includedirs "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
    libdirs "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
end

function includeBlackbone()
    -- compiler
    includedirs "vendor/Blackbone/src"
    -- linker (include lib file and link it)
    filter { "configurations:Debug", "platforms:Win32" }
        libdirs "vendor/Blackbone/build/Win32/Debug"
    filter { "configurations:Debug", "platforms:Win64" }
        libdirs "vendor/Blackbone/build/x64/Debug"
    filter { "configurations:Release", "platforms:Win32" }
        libdirs "vendor/Blackbone/build/Win32/Release"
    filter { "configurations:Release", "platforms:Win64" }
        libdirs "vendor/Blackbone/build/x64/Release"

    filter {}

    links "Blackbone"
end

function includeLegacySdl()
    includedirs "vendor/sdl_1.2/include"
    links "SDL"

    filter { "platforms:Win32" }
        libdirs "vendor/sdl_1.2/lib/x86"
    filter { "platforms:Win64" }
        libdirs "vendor/sdl_1.2/lib/x64"
    filter {}
end

function includePrecompiledHeaders()
    pchheader "pch.h"
    pchsource "projects/%{prj.name}/src/pch.cpp"
end

function includeImGui()
    includedirs "vendor/imgui"
    files {
        "vendor/imgui/*.h",
        "vendor/imgui/*.cpp",
    }
end

-- Framework
project "Icetrix"
    kind "StaticLib"
    characterset "MBCS"

    files {
        "projects/%{prj.name}/src/**",
    }

    includedirs "projects/%{prj.name}/src"
    includeDirectX()
    includeBlackbone();
    includeImGui()

    function includeIcetrix()
        includeBlackbone()
        includedirs "projects/Icetrix/src"
        links "Icetrix"
    end

-- Projects
project "IAssaultCube"
    -- 32bit
    kind "SharedLib"
    characterset "MBCS"

    --includePrecompiledHeaders()
    includeProject()
    includeIcetrix()

    includeLegacySdl()
    includeImGui()
    files {
        "vendor/imgui/examples/imgui_impl_win32.*",
        "vendor/imgui/examples/imgui_impl_opengl2.*",
    }

project "Fallout4"
    -- 64bit
    kind "SharedLib"
    characterset "MBCS"

    --includePrecompiledHeaders()
    includeProject()
    includeIcetrix()

    includeImGui()
    files {
        "vendor/imgui/examples/imgui_impl_win32.*",
        "vendor/imgui/examples/imgui_impl_dx11.*",
    }