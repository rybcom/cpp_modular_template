
solution_name = "solution"
solution_dir ="%{wks.location}"  
platform_specifier = "%{cfg.buildcfg}_%{cfg.platform}"
rundir =solution_dir .."/run/" .. platform_specifier;

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
		"src/%{prj.name}/public",
		"src/%{prj.name}/private",

		"tools/spdlog/include",
		"tools/logger/public",
	}

	configurations 
	{	
		"debug" ,
		"release",
		"profiler" 
	}
	
	files 
	{
		"src/%{prj.name}/**.cpp",
		"src/%{prj.name}/**.h",
	}

	filter "configurations:debug"
  		optimize "Off"
  		defines {"LOGGING_ENABLED=true"}

	filter "configurations:profiler"
  		optimize "Full"
  		defines {"LOGGING_ENABLED=true"}
  		
	filter "configurations:release"
  		optimize "Full"
  		defines {"LOGGING_ENABLED=false"}

	group "mySolution"

	  	----------------------------------------------
	  	--										    --
	  	--					myProject				--
	  	--											--
	  	----------------------------------------------


		project "myProject"
			location "intermediate/project_files"
			kind "ConsoleApp"
			language "C++"
			cppdialect "C++17"
	
			postbuildcommands 
			{
  				"{MKDIR} ../../run/%{cfg.buildcfg}_%{cfg.platform}",
  				"{COPY} %{cfg.buildtarget.abspath} ../../run/%{cfg.buildcfg}_%{cfg.platform}",
			}
		
			defines 
			{
				"UPDATE_USER_INPUT_EVENTS=true",
			}

			includedirs
			{

				"tools/user_input/public",
				"tools/utilities/public",
				"tools/config/public",
				"tools/event_system/public",
			}

			links
			{
				"logger",
				"user_input",
				"utilities",
				"config",
			}

			debugcommand (rundir .. "/%{cfg.buildtarget.name}")
			debugdir (rundir)

	group "tools"

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




		
