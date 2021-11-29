#pragma once
#include "project_config.h"
#include "Utilities.h"
#include "Configuration.h"
#include <string>

#define CONFIGURATION_CLASS(ConfigClass) \
struct ConfigClass : public config::Configuration

inline std::string find_file_path(std::string const& file)
{
	static auto config_arg = aux::get_command_parameter(0).value_or("default");
	set_project_configuration(config_arg);

	return get_init_file(file);
}

#define CONFIGURATION_INIT(ConfigType,InitFile)					\
																\
	public:														\
																\
		ConfigType() : Configuration(find_file_path(InitFile))	\
		{														\
			_configFilePath = InitFile;							\
			initProperties();									\
		}														\
																\
	private:													\
																\
		void initProperties()									\

