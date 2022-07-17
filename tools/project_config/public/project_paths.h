#pragma once

#include <string>
#include <fmt/header_only.h>
#include "Utilities.h"

#pragma region project paths

inline const std::string ProjectStartupDirPath;
inline const std::string ProjectBinDirPath;
inline const std::string ProjectInitDirPath = R"(..\init\)";
inline const std::string ProjectDataDirPath = R"(..\data\)";
inline const std::string ProjectLogDirPath = R"(app_logs\)";

inline std::string ProjectConfiguration;

#pragma endregion

#pragma region api

inline void set_project_configuration(std::string_view configuration)
{
	ProjectConfiguration = configuration;
}

inline std::string get_init_file(std::string_view file)
{
	static auto config_arg = aux::get_command_parameter(0).value_or("default");
	set_project_configuration(config_arg);
	
	return fmt::format(R"({}{}\{})", ProjectInitDirPath, ProjectConfiguration, file);
}




#pragma endregion