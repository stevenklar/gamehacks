-- Defaults
characterset(DEFAULT_CHARACTER_SET)

-- Framework
project "Icetrix"
    kind "StaticLib"

    includeProject()
    includeDirectX()
    includeBlackbone()
    includeImGui()
    includeEntt()

-- Projects
project "IAssaultCube"
    -- 32bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()
    includeLegacySdl()

    files {
        "vendor/imgui/examples/imgui_impl_win32.*",
        "vendor/imgui/examples/imgui_impl_opengl2.*",
    }

project "Fallout4"
    -- 64bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()

    files {
        "vendor/imgui/examples/imgui_impl_win32.*",
        "vendor/imgui/examples/imgui_impl_dx11.*",
    }

project "AgeOfWonders3"
    -- 32bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()

    files {
        "vendor/imgui/examples/imgui_impl_win32.*",
        "vendor/imgui/examples/imgui_impl_dx11.*",
    }

project "DoorKickers"
    -- 32bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()

    files {
        "vendor/imgui/examples/imgui_impl_win32.*",
        "vendor/imgui/examples/imgui_impl_opengl2.*",
    }
