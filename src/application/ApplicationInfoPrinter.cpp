#include "Application.h"
#include "Logger.h"
#include "configs/windowconfig.h"

std::string to_string_log_verbosity(LogVerbosity verbosity)
{
	switch (verbosity)
	{
	case Trace:
		return "Trace";
	case Debug:
		return "Debug";
	case Info:
		return "Info";
	case Warn:
		return "Warn";
	case Error:
		return "Error";
	case Critical:
		return "Critical";
	default:
		return "unknown";
	}
}

void print_logo()
{

	APP_LOG(
		R"(
**********************************************************************************
*																			     *									      
*                                 Sandbox app                                    *
*																			     *									      
**********************************************************************************
)");
}

void print_preprocessor_definitions()
{
	TRACE_FUNCTION();


	CHAR name[256 + 1];
	DWORD length = 256 + 1;

	::GetComputerNameA((CHAR*)name, &length);
	std::string computerNameStr(name);

	::GetUserNameA((CHAR*)name, &length);
	std::string userNameStr(name);

	APP_LOG(
		R"(
___________________________________________

Preprocessor definitions:

    Static configuration : {}
    Configuration variant : {}
	USE_GUI : {}
	DEVEL_MODE_ENABLED : {}
	SANITY_CHECK_ENABLED : {}
	DEBUGGER_ASSERTION_ENABLED : {}

PC info:
	user name			: {}
	pc name				: {}
___________________________________________)",
CONFIGURATION_NAME(),
ProjectConfiguration,
USE_GUI(),
DEVEL_MODE_ENABLED(),
SANITY_CHECK_ENABLED(),
DEBUGGER_ASSERTION_ENABLED(),
userNameStr,
computerNameStr
);

}

void print_window_config()
{
	APP_LOG(
		R"(
___________________________________________

windows settings:

	viewer_height: {} 
	viewer_width : {} 
	viewer_position_x : {}
	viewer_position_y : {}

	is_console_visible : {} 
	is_debug_viewer_maximized : {}
	is_debug_viewer_visible : {}
___________________________________________)",

config::windows.viewer_height,
config::windows.viewer_width,
config::windows.viewer_position_x,
config::windows.viewer_position_y,

config::windows.is_console_visible,
config::windows.is_debug_viewer_maximized,
config::windows.is_debug_viewer_visible
);
}

void print_starting_info()
{
	TRACE_FUNCTION();

	print_logo();
	print_preprocessor_definitions();
	print_window_config();
}


