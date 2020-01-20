#pragma once
#include "project_config.h"

#include "Configuration.h"
#include "Utilities.h"

CONFIGURATION_CLASS(WindowConfig)
{
	ConfigProperty(bool, IsViewerMaximized, true);
	ConfigProperty(bool, IsViewerVisible, true);
	ConfigProperty(bool, IsConsoleVisible, true);
	ConfigProperty(unsigned int, WindowPositionX, 0);
	ConfigProperty(unsigned int, WindowPositionY, 0);
	ConfigProperty(unsigned int, WindowWidth, 800);
	ConfigProperty(unsigned int, WindowHeight, 600);

	CONFIGURATION_INIT(WindowConfig, aux::get_config_file("window_config.ini"))
	{
		IsConsoleVisible = get_bool("console", "is_visible", IsConsoleVisible);

		IsViewerMaximized = get_bool("viewer", "is_maximized", IsViewerMaximized);
		IsViewerVisible = get_bool("viewer", "is_visible", IsViewerVisible);
	
		WindowPositionX = get_unsigned_int("viewer", "position_x", WindowPositionX);
		WindowPositionY = get_unsigned_int("viewer", "position_y", WindowPositionY);
		WindowWidth = get_unsigned_int("viewer", "width", WindowWidth);
		WindowHeight = get_unsigned_int("viewer", "height", WindowHeight);
	}

};

