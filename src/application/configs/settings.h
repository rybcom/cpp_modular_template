#pragma once
#include "project_config.h"
#include "ConfigFileLoader.h"

CONFIGURATION_CLASS(setting_config_t)
{
	std::string welcome_phrase{ "hello" };
	std::string goodbye_phrase{ "bye" };

	CONFIGURATION_INIT(setting_config_t, "settings.ini")
	{
		welcome_phrase = get_string("intro", "welcome_phrase", welcome_phrase);
		goodbye_phrase = get_string("intro", "goodbye_phrase", goodbye_phrase);
	}
};

inline setting_config_t const setting_config;



