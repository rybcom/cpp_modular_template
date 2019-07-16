
#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <memory>


#ifndef LOGGING_ENABLED
	#error "please set preprocessor define LOGGING_ENABLED=true or LOGGING_ENABLED=false "
#endif


namespace logger
{
	void  registerLogger(std::string const & name);
	
	std::shared_ptr<spdlog::logger>&  getLoggerByName(std::string const & name);
	

}

#define DEFINE_LOGGER_AlWAYS(log_name)    ::logger::registerLogger(#log_name)
#define TRACE_LOG_AlWAYS(log_name,...)    ::logger::getLoggerByName(#log_name)->trace(__VA_ARGS__)
#define DEBUG_LOG_AlWAYS(log_name,...)    ::logger::getLoggerByName(#log_name)->debug(__VA_ARGS__)
#define INFO_LOG_AlWAYS(log_name,...)    ::logger::getLoggerByName(#log_name)->info(__VA_ARGS__)
#define WARN_LOG_AlWAYS(log_name,...)    ::logger::getLoggerByName(#log_name)->warn(__VA_ARGS__)
#define ERROR_LOG_AlWAYS(log_name,...)    ::logger::getLoggerByName(#log_name)->error(__VA_ARGS__)
#define CRITICAL_LOG_AlWAYS(log_name,...)    ::logger::getLoggerByName(#log_name)->critical(__VA_ARGS__)


#if LOGGING_ENABLED


	#define DEFINE_LOGGER(log_name)    ::logger::registerLogger(#log_name)

	#define TRACE_LOG(log_name,...)    ::logger::getLoggerByName(#log_name)->trace(__VA_ARGS__)
	#define DEBUG_LOG(log_name,...)    ::logger::getLoggerByName(#log_name)->debug(__VA_ARGS__)
	#define INFO_LOG(log_name,...)    ::logger::getLoggerByName(#log_name)->info(__VA_ARGS__)
	#define WARN_LOG(log_name,...)    ::logger::getLoggerByName(#log_name)->warn(__VA_ARGS__)
	#define ERROR_LOG(log_name,...)    ::logger::getLoggerByName(#log_name)->error(__VA_ARGS__)
	#define CRITICAL_LOG(log_name,...)    ::logger::getLoggerByName(#log_name)->critical(__VA_ARGS__)

#else

	#define DEFINE_LOGGER(log_name)    

	#define TRACE_LOG(log_name,...)    
	#define DEBUG_LOG(log_name,...)    
	#define INFO_LOG(log_name,...)    
	#define WARN_LOG(log_name,...)    
	#define ERROR_LOG(log_name,...)    
	#define CRITICAL_LOG(log_name,...)   
#endif
