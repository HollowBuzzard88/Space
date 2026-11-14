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
    kind "ConsoleApp"
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
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        buildoptions
        {
            "/utf-8"
        }

        defines
        {
        }

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
