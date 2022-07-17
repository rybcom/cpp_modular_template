
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
		"tools/tomlplusplus/public",
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
  		defines {"CONFIGURATION_DEBUG"}

	filter "configurations:profiler"
  		optimize "Full"
  		defines {"CONFIGURATION_PROFILER"}
  		
	filter "configurations:release"
  		optimize "Full"
  		defines {"CONFIGURATION_RELEASE"}

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
				"src/%{prj.name}/**",
			}

			links
			{
				"fmt",
				"imgui",
				"user_input",
				"utilities",
				"stopwatch"
			}

			debugargs 
			{
				"variant_a",
				"test_argument_2",
				"test_argument_3",
			}

			postbuildcommands 
			{
				"{MKDIR} ../../bin/",
				"{COPY} %{cfg.buildtarget.abspath} ../../bin/",
			}

			debugcommand (solution_dir .."/bin/" .. "%{cfg.buildtarget.name}")
			debugdir (solution_dir .."/bin")

			filter "configurations:debug"
				targetname ("application_debug")
			filter "configurations:profiler"
				targetname ("application_profiler")
			filter "configurations:release"
				targetname ("application_release")

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
		  	--					tomlplusplus            --
		  	--											--
		  	----------------------------------------------

			project "tomlplusplus"
				location "intermediate/project_files"
				kind "none"
				language "C++"
				cppdialect "C++17"
			
				includedirs
				{
					"tools/%{prj.name}/public",
				}

				files 
				{
					"tools/%{prj.name}/**.inl",
					"tools/%{prj.name}/**.h",
				}

		
		group ""
	 	



		
