
solution_name = "solution"
solution_dir ="%{wks.location}"  
platform_specifier = "%{cfg.buildcfg}_%{cfg.platform}"

workspace (solution_name)
	
	architecture "x64"
	platforms "win64"
	startproject "simulation"


	targetdir ("intermediate/bin/" .. platform_specifier .. "/%{prj.name}")
	objdir ("intermediate/obj/" .. platform_specifier .. "/%{prj.name}")

	filter "system:windows"
			systemversion "latest"


	includedirs
	{
		"tools/project_config/public",
		"tools/fmt/public",
		"tools/logger/public",
		"tools/user_input/public",
		"tools/utilities/public",
		"tools/config/public",
		"tools/event_system/public",
		"tools/stopwatch/public",
		"tools/catch2/public",
		"tools/imgui/public",
	}

	configurations 
	{	
		"debug" ,
		"release",
		"profiler" 
	}
	
	

	filter "configurations:debug"
  		optimize "Off"
  		defines {
  				"CONFIGURATION_DEBUG=true",
  				"CONFIGURATION_PROFILER=false",
  				"CONFIGURATION_RELEASE=false",
  				}

	filter "configurations:profiler"
  		optimize "Full"
  		defines {
  				"CONFIGURATION_DEBUG=false",
  				"CONFIGURATION_PROFILER=true",
  				"CONFIGURATION_RELEASE=false",
  				}
  		
	filter "configurations:release"
  		optimize "Full"
  		defines {
  				"CONFIGURATION_DEBUG=false",
  				"CONFIGURATION_PROFILER=false",
  				"CONFIGURATION_RELEASE=true",
  				}


group "app"

		----------------------------------------------
	  	--										    --
	  	--					project_config			--
	  	--											--
	  	----------------------------------------------

		project "project_config"
			location "intermediate/project_files"
			kind "None"
			language "C++"
			cppdialect "C++17"

			files
			{
				"tools/%{prj.name}/public/*",
			}


	  	----------------------------------------------
	  	--										    --
	  	--					applicatoin				--
	  	--											--
	  	----------------------------------------------


		project "application"
			location "intermediate/project_files"
			kind "ConsoleApp"
			language "C++"
			cppdialect "C++17"
	

			includedirs
			{
				"src/%{prj.name}",
			}

			files 
			{
				"src/%{prj.name}/**.cpp",
				"src/%{prj.name}/**.h",
			}

			links
			{
				"fmt",
				"imgui",
				"user_input",
				"config",
				"utilities",
				"stopwatch"
			}

			debugargs 
			{
				"test_argument_1",
				"test_argument_2",
				"test_argument_3",
			}


			filter "configurations:debug"
		  		defines 
		  		{
		  			"UPDATE_USER_INPUT_EVENTS=true",
		  		}
				postbuildcommands 
				{
	  				"{MKDIR} ../../run64_debug/",
	  				"{COPY} %{cfg.buildtarget.abspath} ../../run64_debug/",
				}
				debugcommand (solution_dir .."/run64_debug/" .. "%{cfg.buildtarget.name}")
				debugdir (solution_dir .."/run64_debug")

			filter "configurations:profiler"
		  		defines 
		  		{
		  			"UPDATE_USER_INPUT_EVENTS=false",
		  		}
		  		postbuildcommands 
				{
	  				"{MKDIR} ../../run64_profiler/",
	  				"{COPY} %{cfg.buildtarget.abspath} ../../run64_profiler/",
				}
		  		debugcommand (solution_dir .."/run64_profiler/" .. "%{cfg.buildtarget.name}")
				debugdir (solution_dir .."/run64_profiler")
			
			filter "configurations:release"
		  		defines 
		  		{
		  			"UPDATE_USER_INPUT_EVENTS=false",
		  		}
		  		postbuildcommands 
				{
	  				"{MKDIR} ../../run64_release/",
	  				"{COPY} %{cfg.buildtarget.abspath} ../../run64_release/",
				}
		  		debugcommand (solution_dir .."/run64_release/" .. "%{cfg.buildtarget.name}")
				debugdir (solution_dir .."/run64_release")

group "tools"


			----------------------------------------------
		  	--										    --
		  	--					imgui    				--
		  	--											--
		  	----------------------------------------------

			project "imgui"
				location "intermediate/project_files"
				kind "StaticLib"
				language "C++"
				cppdialect "C++17"

				includedirs
				{
					"tools/%{prj.name}/public",
					"tools/%{prj.name}/private",
				}

				files 
				{
					"tools/%{prj.name}/**.cpp",
					"tools/%{prj.name}/**.h",
				}

				links
				{
					"d3d11.lib","d3dcompiler.lib","dxgi.lib",
				}
			----------------------------------------------
		  	--										    --
		  	--					fmt      				--
		  	--											--
		  	----------------------------------------------

			project "fmt"
				location "intermediate/project_files"
				kind "StaticLib"
				language "C++"
				cppdialect "C++17"

				includedirs
				{
					"tools/%{prj.name}/public",
					"tools/%{prj.name}/private",
				}

				files 
				{
					"tools/%{prj.name}/**.cc",
					"tools/%{prj.name}/**.h",
				}

			----------------------------------------------
		  	--										    --
		  	--					event_system			--
		  	--											--
		  	----------------------------------------------

			project "event_system"
				location "intermediate/project_files"
				kind "None"

				files 
				{
					"tools/%{prj.name}/**.cpp",
					"tools/%{prj.name}/**.h",
				}

			----------------------------------------------
		  	--										    --
		  	--					utilities				--
		  	--											--
		  	----------------------------------------------

			project "utilities"
				location "intermediate/project_files"
				kind "StaticLib"
				language "C++"
				cppdialect "C++17"

				includedirs
				{
					"tools/%{prj.name}/public",
					"tools/%{prj.name}/private",
				}

				files 
				{
					"tools/%{prj.name}/**.cpp",
					"tools/%{prj.name}/**.h",
				}


			----------------------------------------------
		  	--										    --
		  	--					user_input				--
		  	--											--
		  	----------------------------------------------

			project "user_input"
				location "intermediate/project_files"
				kind "StaticLib"
				language "C++"
				cppdialect "C++17"

				includedirs
				{
					"tools/%{prj.name}/public",
					"tools/%{prj.name}/private",
				}

				files 
				{
					"tools/%{prj.name}/**.cpp",
					"tools/%{prj.name}/**.h",
				}

		
			----------------------------------------------
		  	--										    --
		  	--					logger					--
		  	--											--
		  	----------------------------------------------

			project "logger"
				location "intermediate/project_files"
				kind "None"
				language "C++"
				cppdialect "C++17"
			
				includedirs
				{
					"tools/%{prj.name}/public",
				}

				files 
				{
					"tools/%{prj.name}/**.cpp",
					"tools/%{prj.name}/**.h",
				}

			----------------------------------------------
		  	--										    --
		  	--					stopwatch				--
		  	--											--
		  	----------------------------------------------

			project "stopwatch"
				location "intermediate/project_files"
				kind "StaticLib"
				language "C++"
				cppdialect "C++17"
			
				includedirs
				{
					"tools/%{prj.name}/public",
				}

				files 
				{
					"tools/%{prj.name}/**.cpp",
					"tools/%{prj.name}/**.h",
				}
				
			----------------------------------------------
		  	--										    --
		  	--					config 					--
		  	--											--
		  	----------------------------------------------

			project "config"
				location "intermediate/project_files"
				kind "StaticLib"
				language "C++"
				cppdialect "C++17"
			
				includedirs
				{
					"tools/%{prj.name}/public",
					"tools/%{prj.name}/private",
				}

				files 
				{
					"tools/%{prj.name}/**.c",
					"tools/%{prj.name}/**.cpp",
					"tools/%{prj.name}/**.h",
				}

		

		
		group ""
	 	



		
