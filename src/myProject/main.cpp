
#include <iostream>

#include "Logger.h"
#include "Configuration.h"
#include "KeyboardEnums.h"
#include "Keyboard.h"

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

	bool bExit = false;

	void key_event_action(user_input::Key key)
	{
		if (key == user_input::Key::Escape)
		{
			INFO_LOG(main, "handled request to terminate");

			bExit = true;
		}
	};


}

int main()
{
	DEFINE_LOGGER(main);

	WARN_LOG_AlWAYS(main, "welcome : {}", settings.getWelocomePhrase());
	WARN_LOG_AlWAYS(main, "press ESC to continue ...");

	user_input::registerHandlerFor_KeyPressed(key_event_action);

	while (!bExit)
	{
		user_input::updateKeyboardState();
	}

	WARN_LOG_AlWAYS(main, "bye-bye");

}
