#pragma once

#include "project_config.h"

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
		
		window_config_t();
	};

	inline window_config_t const windows;
}
