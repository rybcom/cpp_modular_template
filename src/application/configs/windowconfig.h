#pragma once
#include "project_config.h"
#include "ConfigFileLoader.h"

CONFIGURATION_CLASS(window_config_t)
{
	bool is_debug_viewer_maximized{ true };
	bool is_debug_viewer_visible{ true };
	bool is_console_visible{ true };
	int viewer_position_x{ 0 };
	int viewer_position_y{ 0 };
	int viewer_width{ 0 };
	int viewer_height{ 0 };

	CONFIGURATION_INIT(window_config_t, "window_config.ini")
	{
		is_console_visible = get_bool("console", "is_visible", is_console_visible);

		is_debug_viewer_maximized = get_bool("viewer", "is_maximized", is_debug_viewer_maximized);
		is_debug_viewer_visible = get_bool("viewer", "is_visible", is_debug_viewer_visible);

		viewer_position_x = get_unsigned_int("viewer", "position_x", viewer_position_x);
		viewer_position_y = get_unsigned_int("viewer", "position_y", viewer_position_y);
		viewer_width = get_unsigned_int("viewer", "width", viewer_width);
		viewer_height = get_unsigned_int("viewer", "height", viewer_height);
	}

};

inline window_config_t const window_config;


