#pragma once

#include "project_config.h"


namespace config
{
	struct setting_config_t
	{
		std::string welcome_phrase{ "hello" };
		std::string goodbye_phrase{ "bye" };

		setting_config_t();
	};

	inline setting_config_t const settings;

}


