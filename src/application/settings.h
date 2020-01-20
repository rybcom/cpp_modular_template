#pragma once

#include "Configuration.h"
#include "Utilities.h"

namespace config
{
	CONFIGURATION_CLASS(SettingsConfig)
	{
		std::string welcome_phrase{ "hello" };
		std::string goodbye_phrase{ "bye" };

		CONFIGURATION_INIT(SettingsConfig, aux::get_config_file("settings.ini"))
		{
			welcome_phrase = get_string("intro", "welcome_phrase", welcome_phrase);
			goodbye_phrase = get_string("intro", "goodbye_phrase", goodbye_phrase);
		}
	};

	inline SettingsConfig const settings;
}


