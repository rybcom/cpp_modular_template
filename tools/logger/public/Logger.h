
#pragma once

#include <fmt/header_only.h>
#include <fstream>
#include <string>
#include "EventSystem.h"
#include "LoggerVerbosity.h"


namespace file_logger
{
	inline std::ofstream output_stream;
	inline std::string output_path;

	inline void set_logger_file_output(std::string_view file)
	{
		static bool has_been_set = false;

		if (has_been_set == false)
		{
			output_path = file;
			output_stream.open(file.data());
			has_been_set = true;
		}
	}

	inline std::string get_logger_file_output()
	{
		return output_path;
	}

	inline void log(const std::string& text)
	{
		if (output_stream.is_open())
		{
			output_stream << text << '\n';
			output_stream.flush();
		}
	}

	inline void close()
	{
		output_stream.close();
	}
}


namespace log_notification
{
	namespace events
	{
		inline  DECLARE_EVENT_TwoParams(LogEvent, LogVerbosity ,verbosity,std::string_view, text_log);
	};
}

#define IS_VERBOSITY_ACCEPET(verbosity) (LOG_VERBOSITY()<=LogVerbosity::##verbosity) 

#define TRACE_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Trace))   {fmt::print(fmt::fg(fmt::color::gray),std::string(log_msg)+"\n",__VA_ARGS__);} \
if constexpr (LOGGING_ENABLED_FILE() && IS_VERBOSITY_ACCEPET(Trace)) { file_logger::log(fmt::format(std::string("[trace] ").append(std::string(log_msg)), __VA_ARGS__).c_str()); } \
if constexpr (LOGGING_ENABLED_EVENTS() && IS_VERBOSITY_ACCEPET(Trace)) { log_notification::events::LogEvent(LogVerbosity::Trace, fmt::format(std::string(log_msg), __VA_ARGS__)); }

#define DEBUG_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_CONSOLE() &&  IS_VERBOSITY_ACCEPET(Debug))   {fmt::print(fmt::fg(fmt::color::white),std::string(log_msg)+"\n",__VA_ARGS__);} \
if constexpr (LOGGING_ENABLED_FILE() && IS_VERBOSITY_ACCEPET(Debug)) { file_logger::log(fmt::format(std::string("[debug] ").append(std::string(log_msg)), __VA_ARGS__).c_str()); } \
if constexpr (LOGGING_ENABLED_EVENTS() && IS_VERBOSITY_ACCEPET(Debug)) { log_notification::events::LogEvent(LogVerbosity::Debug, fmt::format(std::string(log_msg), __VA_ARGS__)); }

#define INFO_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Info))    {fmt::print(fmt::fg(fmt::color::light_green),std::string(log_msg)+"\n",__VA_ARGS__);} \
if constexpr (LOGGING_ENABLED_FILE() && IS_VERBOSITY_ACCEPET(Info)) { file_logger::log(fmt::format(std::string("[info] ").append(std::string(log_msg)), __VA_ARGS__).c_str()); } \
if constexpr (LOGGING_ENABLED_EVENTS() && IS_VERBOSITY_ACCEPET(Info)) { log_notification::events::LogEvent(LogVerbosity::Info, fmt::format(std::string(log_msg), __VA_ARGS__)); }

#define WARN_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Warn))    {fmt::print(fmt::fg(fmt::color::yellow),std::string(log_msg)+"\n",__VA_ARGS__);} \
if constexpr (LOGGING_ENABLED_FILE() && IS_VERBOSITY_ACCEPET(Warn)) { file_logger::log(fmt::format(std::string("[warn] ").append(std::string(log_msg)), __VA_ARGS__).c_str()); } \
if constexpr (LOGGING_ENABLED_EVENTS() && IS_VERBOSITY_ACCEPET(Warn)) { log_notification::events::LogEvent(LogVerbosity::Warn, fmt::format(std::string(log_msg), __VA_ARGS__)); }

#define ERROR_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Error))   {fmt::print(fmt::fg(fmt::color::tomato),std::string(log_msg)+"\n",__VA_ARGS__);}  \
if constexpr (LOGGING_ENABLED_FILE() && IS_VERBOSITY_ACCEPET(Error)) { file_logger::log(fmt::format(std::string("[error] ").append(std::string(log_msg)), __VA_ARGS__).c_str()); } \
if constexpr (LOGGING_ENABLED_EVENTS() && IS_VERBOSITY_ACCEPET(Error)) { log_notification::events::LogEvent(LogVerbosity::Error, fmt::format(std::string(log_msg), __VA_ARGS__));  }

#define CRITICAL_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Critical))   {fmt::print(fmt::fg(fmt::color::red),std::string(log_msg)+"\n",__VA_ARGS__);} \
if constexpr (LOGGING_ENABLED_FILE() && IS_VERBOSITY_ACCEPET(Critical)) { file_logger::log(fmt::format(std::string("[critical] ").append(std::string(log_msg)), __VA_ARGS__).c_str()); } \
if constexpr (LOGGING_ENABLED_EVENTS() && IS_VERBOSITY_ACCEPET(Critical)) { log_notification::events::LogEvent(LogVerbosity::Critical, fmt::format(std::string(log_msg), __VA_ARGS__));  }

#define APP_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Application))   {fmt::print(fmt::fg(fmt::color::light_blue),std::string(log_msg)+"\n",__VA_ARGS__);} \
if constexpr (LOGGING_ENABLED_FILE() && IS_VERBOSITY_ACCEPET(Application)) { file_logger::log(fmt::format(std::string("[app] ").append(std::string(log_msg)), __VA_ARGS__).c_str()); } \
if constexpr (LOGGING_ENABLED_EVENTS() && IS_VERBOSITY_ACCEPET(Application)) { log_notification::events::LogEvent(LogVerbosity::Application, fmt::format(std::string(log_msg), __VA_ARGS__));  }


#define  LOG_TRACE_FUNCTION()  TRACE_LOG(__FUNCSIG__)


