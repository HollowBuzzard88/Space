require("C:/dev/premake-vscode/vscode")

workspace "Space"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    
    startproject "Space"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Space"
    location "Space"
    kind "WindowedApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/Source"
    }

    links
    {
        "opengl32.lib",
        "dwmapi.lib"
    }

    filter "system:windows"
        defines "SP_PLATFORM_WINDOWS"
        cppdialect "C++23"
        staticruntime "On"
        systemversion "latest"

        entrypoint "mainCRTStartup"

        buildoptions
        {
            "/utf-8"
        }

        defines
        {
        }
    filter "system:linux"
        defines "SP_PLATFORM_LINUX"
    filter "system:macosx"
        defines "SP_PLATFORM_MAC"

    filter "configurations:Debug"
        staticruntime "off"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        staticruntime "off"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        staticruntime "off"
        runtime "Release"
        optimize "On"
