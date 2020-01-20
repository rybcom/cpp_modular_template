
#include <iostream>

#include "project_config.h"
#include "Logger.h"
#include "Configuration.h"
#include "KeyboardEnums.h"
#include "Keyboard.h"
#include "Ranges.h"
#include <vector>
#include <string>

#if (RUN_WITH_GUI() ==  false && RUN_AS_UNIT_TESTING() == false) 

namespace
{
	bool bExit = false;
	std::vector<std::string> words = { "JULY","MOUSE","TABLE","CAP","WATER","JOHN","TORONTO","CAR" };

	class SettingsConfig : public config::Configuration
	{
		ConfigProperty(std::string, WelocomePhrase, "hello");

		CONFIGURATION_INIT(SettingsConfig, R"(..\init\settings.ini)")
		{
			WelocomePhrase = get_string("intro", "welcome_phrase", WelocomePhrase);
		}
	} settings;


	auto constains_key_char(user_input::Key key)
	{
		return [key](std::string const & word)
		{
			return word.find(char(key)) != std::string::npos;
		};
	}

	auto print_word(user_input::Key key)
	{
		return [key](std::string const & word)
		{
			INFO_LOG("word: {} contain key {}", word, char(key));
		};
	}

	void check_key_presence(user_input::Key key)
	{
		words | views::filter<std::string>(constains_key_char(key))
			| views::for_each<std::string>(print_word(key));
	}

	void key_event_action(user_input::Key key)
	{
		if (key == user_input::Key::Escape)
		{
			INFO_LOG("handled request to terminate");

			bExit = true;
		}
		else
		{
			check_key_presence(key);
		}
	};

}

int main()
{

	INFO_LOG_ALWAYS("welcome : {}", settings.getWelocomePhrase());
	INFO_LOG_ALWAYS("press ESC to exit ...");

	user_input::registerHandlerFor_KeyUp(key_event_action);

	while (!bExit)
	{
		user_input::updateKeyboardState();
	}

	INFO_LOG_ALWAYS("bye-bye");

}

#endif

