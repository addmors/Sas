project "GLad"
	kind "StaticLib"
	language "C"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	filter "system:windows"
		 buildoptions { "-std=c11", "-lgdi32" , "/MT"}
            	systemversion "latest"
            	staticruntime "On"
	
	includedirs {
		"include"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"