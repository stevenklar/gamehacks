-- Functions
function includeProject()
    includedirs(PROJECTS_DIR .. "%{prj.name}/src")
    files(PROJECTS_DIR .. "%{prj.name}/src/**")
end

function includeDirectX()
    includedirs(DIRECTX_DIR .. "Include")

    filter { "configurations:Debug", "platforms:Win32" }
        libdirs(DIRECTX_DIR .. "%{dxDir}Lib/x86")
    filter { "configurations:Debug", "platforms:Win64" }
        libdirs(DIRECTX_DIR .. "%{dxDir}Lib/x64")
    filter {}
end

function includeBlackbone()
    includedirs(VENDOR_DIR .. "Blackbone/src")

    filter { "configurations:Debug", "platforms:Win32" }
        libdirs(VENDOR_DIR .. "Blackbone/build/Win32/Debug")
    filter { "configurations:Debug", "platforms:Win64" }
        libdirs(VENDOR_DIR .. "Blackbone/build/x64/Debug")

    filter { "configurations:Release", "platforms:Win32" }
        libdirs(VENDOR_DIR .. "Blackbone/build/Win32/Release")
    filter { "configurations:Release", "platforms:Win64" }
        libdirs(VENDOR_DIR .. "Blackbone/build/x64/Release")

    filter {}

    links "Blackbone"
end

function includeLegacySdl()
    includedirs(VENDOR_DIR .. "sdl_1.2/include")
    links "SDL"

    filter { "platforms:Win32" }
        libdirs(VENDOR_DIR .. "sdl_1.2/lib/x86")
    filter { "platforms:Win64" }
        libdirs(VENDOR_DIR .. "sdl_1.2/lib/x64")
    filter {}
end

function includePrecompiledHeaders()
    pchheader("pch.h")
    pchsource(PROJECTS_DIR .. "%{prj.name}/src/pch.cpp")
end

function includeImGui()
    includedirs(VENDOR_DIR .. "imgui")
    files {
        VENDOR_DIR .. "imgui/*.h",
        VENDOR_DIR .. "imgui/*.cpp",
    }
end

function includeEntt()
    includedirs(VENDOR_DIR .. "entt")
    files(VENDOR_DIR .. "entt/entt.hpp")
end

function includeIcetrix()
    includeBlackbone()
    includeImGui()
    includeEntt()
    includeLegacySdl()

    includedirs(PROJECTS_DIR .. "Icetrix/src")
    links "Icetrix"
end