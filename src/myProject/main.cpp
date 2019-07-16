
#include <iostream>
#include "Logger.h"
#include "Configuration.h"

namespace 
{
	class SettingsConfig : public config::Configuration
	{
		ConfigProperty_DefaultValue(std::string, WelocomePhrase, "hello");

		CONFIGURATION_INIT(SettingsConfig, R"(..\..\config\settings.ini)")
		{
			WelocomePhrase = get_string("intro", "welcome_phrase", WelocomePhrase);
		}
	} settings ;
}

int main()
{
	DEFINE_LOGGER(main);

	WARN_LOG_AlWAYS(main, "Welcome : {}",settings.getWelocomePhrase());
}
