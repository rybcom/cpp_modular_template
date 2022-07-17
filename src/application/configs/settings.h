#pragma once

#include "project_config.h"
#include "toml.h"
#include <string_view>


namespace config
{
	struct setting_config_t
	{
		std::string welcome_phrase{ "hello" };
		std::string goodbye_phrase{ "bye" };

		setting_config_t()
		{
			std::string config_file_path = get_init_file("settings.ini");
			toml::table tbl = toml::parse_file(config_file_path);

			welcome_phrase = tbl["intro"]["welcome_phrase"].value_or(welcome_phrase);
			goodbye_phrase = tbl["intro"]["goodbye_phrase"].value_or(goodbye_phrase);

		}
	};

	inline setting_config_t const settings;

}


