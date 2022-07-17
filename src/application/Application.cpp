#include "Application.h"

#if USE_GUI()

#include "debug_ui/DebugViewer.h"

#define UPDATE_DEBUG_VIEWER() update_debug_viewer()
#define INIT_DEBUG_VIEWER() 	debug_ui::init()
#define CLOSE_DEBUG_VIEWER() 	debug_ui::close()
#else
#define UPDATE_DEBUG_VIEWER() 
#define INIT_DEBUG_VIEWER() 
#define CLOSE_DEBUG_VIEWER()
#endif
#include "Utilities.h"
#include <filesystem>


void print_starting_info();

#if USE_GUI() == true

void update_debug_viewer()
{
	TRACE_FUNCTION();

	if (debug_ui::update() == debug_ui::DebugViewerResult::RequestClose)
	{
		events::request_close_application();
	}
}

#endif

namespace
{
	DISABLE_WARNING_PUSH()
		DISABLE_WARNING_DEPRECATED_MEMBERS()

		void init_project_configuration()
	{
		static auto config_arg = aux::get_command_parameter(0).value_or("default");
		set_project_configuration(config_arg);
	}

	std::string get_file_logger_name_and_crate_log_directory()
	{
		TRACE_FUNCTION();

		std::time_t t = std::time(nullptr);
		auto& time = *std::localtime(&t);

		auto folderDate = fmt::format("{:%Y-%m-%d}", time);
		auto dirPath = fmt::format("{}{}", ProjectLogDirPath, folderDate);
		std::filesystem::create_directories(dirPath);

		auto fileName = fmt::format("{:%Hhod_%Mmin}_log.info", time);
		return fmt::format("{}\\{}", dirPath, fileName);
	}


	DISABLE_WARNING_POP()
}


void pre_initialization()
{
	init_project_configuration();

	INIT_DEBUG_VIEWER();

#if LOGGING_ENABLED_FILE() == true
	file_logger::set_logger_file_output(get_file_logger_name_and_crate_log_directory());
#endif

	print_starting_info();
}

SandboxApp& SandboxApp::get()
{
	static SandboxApp instance;
	return instance;
}

void SandboxApp::init()
{
	TRACE_FUNCTION();

	pre_initialization();

	APP_LOG("init sequence started");

	for (auto const& modulPtr: _modules)
	{
		modulPtr->init();
	}

	APP_LOG("init sequence finished");
}

void SandboxApp::update()
{
	UPDATE_DEBUG_VIEWER();

	TRACE_FUNCTION();

	_deltaTime = 0.016666667;
	_globalTime += _deltaTime;

	for (auto const& modulPtr : _modules)
	{
		modulPtr->update();
	}
}

void SandboxApp::close()
{
	TRACE_FUNCTION();

	APP_LOG("closing sequence started");

	for (auto const& modulPtr : _modules)
	{
		modulPtr->close();
	}

	APP_LOG("closing sequence finished");

#if LOGGING_ENABLED_FILE() == true
	file_logger::close();
#endif

	CLOSE_DEBUG_VIEWER();
}

double SandboxApp::getDeltaTime() const
{
	return _deltaTime;
}

double SandboxApp::getGlobalTime() const
{
	return _globalTime;
}

