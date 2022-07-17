#pragma once

#include "project_config.h"
#include "toml.h"
#include "Utilities.h"
#include <string_view>

inline std::string find_file_path(std::string const& file)
{
	static auto config_arg = aux::get_command_parameter(0).value_or("default");
	set_project_configuration(config_arg);
	return get_init_file(file);
}

namespace config
{
	struct setting_config_t
	{
		std::string welcome_phrase{ "hello" };
		std::string goodbye_phrase{ "bye" };

		setting_config_t()
		{
			std::string config_file_path = find_file_path("settings.ini");
			toml::table tbl = toml::parse_file(config_file_path);

			welcome_phrase = tbl["intro"]["welcome_phrase"].value_or(welcome_phrase);
			goodbye_phrase = tbl["intro"]["goodbye_phrase"].value_or(goodbye_phrase);

		}
	};

	inline setting_config_t const settings;

}


