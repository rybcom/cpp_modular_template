

#include "settings.h"
#include "windowconfig.h"

#include "toml.h"
#include <string_view>

namespace config
{

	setting_config_t::setting_config_t()
	{
		std::string config_file_path = get_init_file("settings.ini");
		toml::table tbl = toml::parse_file(config_file_path);

		welcome_phrase = tbl["intro"]["welcome_phrase"].value_or(welcome_phrase);
		goodbye_phrase = tbl["intro"]["goodbye_phrase"].value_or(goodbye_phrase);

	}

	window_config_t::window_config_t()
	{
		std::string config_file_path = get_init_file("window_config.ini");
		toml::table tbl = toml::parse_file(config_file_path);

		is_debug_viewer_maximized = tbl["viewer"]["is_debug_viewer_maximized"].value_or(is_debug_viewer_maximized);
		is_debug_viewer_visible = tbl["viewer"]["is_visible"].value_or(is_debug_viewer_visible);
		is_console_visible = tbl["console"]["is_visible"].value_or(is_console_visible);

		viewer_position_x = tbl["viewer"]["position_y"].value_or(viewer_position_x);
		viewer_position_y = tbl["viewer"]["position_y"].value_or(viewer_position_y);
		viewer_width = tbl["viewer"]["width"].value_or(viewer_width);
		viewer_height = tbl["viewer"]["height"].value_or(viewer_height);
	}
}
