solution "arbarlith2"
    configurations { "Debug", "Release" }

    project "arbarlith2"
        kind "ConsoleApp"
        language "C++"
        files { "**.h", "**.cpp" } -- recurse into subdirectories
		targetname "arbarlith2"
		targetdir "./bin/"

        configuration "windows"  
            links { "opengl32",
            		"glu32",
            		"glut32",
            		"glew32",
            		"DevIL",
            		"SDL",
					"SDL_mixer",
            		"ILU",
            		"ILUT" }
                    
        configuration "linux"
        	links { "GL",
                    "GLU",
            		"GLEW",
            		"SDL",
					"SDL_mixer",
            		"IL",
            		"ILU",
            		"ILUT" }
                    
        configuration "gmake"
            includedirs { "." }

        configuration { "gmake", "Debug" }
            buildoptions { "-ggdb" }
            linkoptions { "-rdynamic" }

        configuration "vs2008"
            includedirs { "$(ProjectDir)", "$(ProjectDir)include-win32" }
            libdirs { "$(ProjectDir)lib" }
            linkoptions { "/NODEFAULTLIB:LIBCMT" }
            
        configuration { "vs2008", "Debug" } 
            linkoptions { "/NODEFAULTLIB:MSVCRT" }
            flags { "Symbols" }

        configuration "Debug"
            defines { "DEBUG" }
            -- flags { "Symbols" }
            targetprefix "DEBUG-"
        
        configuration "Release"
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
