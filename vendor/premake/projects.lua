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
    includeLegacySdl()

-- Projects
project "IAssaultCube"
    -- 32bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()

    files {
        VENDOR_DIR .. "imgui/examples/imgui_impl_win32.*",
        VENDOR_DIR .. "imgui/examples/imgui_impl_opengl2.*",
    }

project "Fallout4"
    -- 64bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()
    includeFw1();

    files {
        VENDOR_DIR .. "imgui/examples/imgui_impl_win32.*",
        VENDOR_DIR .. "imgui/examples/imgui_impl_dx11.*",
    }

project "Thief"
    -- 64bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()

    files {
        VENDOR_DIR .. "imgui/examples/imgui_impl_win32.*",
        VENDOR_DIR .. "imgui/examples/imgui_impl_dx11.*",
    }

project "MotorsportManager"
    -- 64bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()

    files {
        VENDOR_DIR .. "imgui/examples/imgui_impl_win32.*",
        VENDOR_DIR .. "imgui/examples/imgui_impl_dx11.*",
    }


project "AgeOfWonders3"
    -- 32bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()

    files {
        VENDOR_DIR .. "imgui/examples/imgui_impl_win32.*",
        VENDOR_DIR .. "imgui/examples/imgui_impl_dx11.*",
    }

project "DoorKickers"
    -- 32bit
    kind "SharedLib"

    includeProject()
    includeIcetrix()

    files {
        VENDOR_DIR .. "imgui/examples/imgui_impl_win32.*",
        --VENDOR_DIR .. "imgui/examples/imgui_impl_opengl2.*",
    }

--[[ Test externalproject
externalproject "BlackBone"
   location(VENDOR_DIR .. "Blackbone")
   uuid "A2C53563-46F5-4D87-903F-3F1F2FDB2DEB"
   kind "StaticLib"
]]
