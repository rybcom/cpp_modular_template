#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h" 

namespace logger
{
	void  registerLogger(std::string const & name)
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		spdlog::stdout_color_mt(name)->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>&  getLoggerByName(std::string const & name)
	{
		return spdlog::get(name);
	}
}