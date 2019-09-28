-- Prepare some dynamic variables
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Workspace
workspace "GameHacks"
    location ("build/gen")
    targetdir("build/bin/%{prj.name}/%{cfg.longname}")
    objdir   ("build/obj/%{prj.name}/%{cfg.longname}")
    defines { "_CRT_SECURE_NO_WARNINGS" }

    platforms      "Win32"
    architecture   "x86"

    language       "C++"
    cppdialect     "C++latest"

    configurations { "Debug", "Release" }

    filter { "configurations:Debug" }
        symbols "On"

    filter { "configurations:Release" }
        optimize "On"

    filter {}

    flags {
        "MultiProcessorCompile"
    }

-- Helper
function includeDirectX()
    includedirs "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
    libdirs "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
end

function includeBlackbone()
    -- compiler
    includedirs "vendor/Blackbone/src"
    -- linker (include lib file and link it)
    libdirs "vendor/Blackbone/build/Win32/Debug"
    links "Blackbone"
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

    function includeIcetrix()
        -- compiler
        includedirs "projects/Icetrix/src"
        -- linker (include lib file and link it)
        links "Icetrix"
    end

-- Projects
project "IAssaultCube"
    kind "SharedLib"
    characterset "MBCS"

    defines {
        "WINDOWS_IGNORE_PACKING_MISMATCH"
    }

    files {
        "projects/%{prj.name}/src/**",
    }

    -- imgui
    files {
        "projects/%{prj.name}/vendor/imgui/**.h",
        "projects/%{prj.name}/vendor/imgui/**.cpp",
    }
    -- vendors
    includedirs "projects/%{prj.name}/vendor"

    --links "dwmapi"
    -- include own path to use project relative includes
    includedirs "projects/%{prj.name}/src"
    --includedirs "projects/%{prj.name}/vendor"
    includedirs "projects/%{prj.name}/vendor/imgui/misc/sdl_1.2/include"
    libdirs "projects/%{prj.name}/vendor/imgui/misc/sdl_1.2/lib/x86"
    links "SDL"
    includeBlackbone()
    includeIcetrix();

project "PAssaultCube" -- Loader for IAssaultCube
    kind "ConsoleApp"

    files {
        "projects/%{prj.name}/src/**",
    }

    includeBlackbone()

--[[
project "EAssaultCube"
    kind "WindowedApp"

    files {
        "projects/%{prj.name}/src/**",
    }

    includeDirectX()
    includeIcetrix();
]]
