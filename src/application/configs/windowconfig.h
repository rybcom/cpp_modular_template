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
	struct window_config_t
	{
		bool is_debug_viewer_maximized{ true };
		bool is_debug_viewer_visible{ true };
		bool is_console_visible{ true };
		int viewer_position_x{ 0 };
		int viewer_position_y{ 0 };
		int viewer_width{ 0 };
		int viewer_height{ 0 };

		window_config_t()
		{
			std::string config_file_path = find_file_path("window_config.ini");
			toml::table tbl = toml::parse_file(config_file_path);

			is_debug_viewer_maximized = tbl["viewer"]["is_debug_viewer_maximized"].value_or(is_debug_viewer_maximized);
			is_debug_viewer_visible = tbl["viewer"]["is_visible"].value_or(is_debug_viewer_visible);
			is_console_visible = tbl["console"]["is_visible"].value_or(is_console_visible);

			viewer_position_x = tbl["viewer"]["position_y"].value_or(viewer_position_x);
			viewer_position_y = tbl["viewer"]["position_y"].value_or(viewer_position_y);
			viewer_width = tbl["viewer"]["width"].value_or(viewer_width);
			viewer_height = tbl["viewer"]["height"].value_or(viewer_height);
		}
	};

	inline window_config_t const windows;
}
