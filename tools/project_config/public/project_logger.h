#pragma once

#include "LoggerVerbosity.h"
#include "Logger.h"

#if defined(CONFIGURATION_DEBUG)

#define LOG_VERBOSITY() LogVerbosity::Debug
#define LOGGING_ENABLED_EVENTS() true
#define LOGGING_ENABLED_CONSOLE() true
#define LOGGING_ENABLED_FILE() true

#endif

#if defined(CONFIGURATION_PROFILER)

#define LOG_VERBOSITY() LogVerbosity::Info
#define LOGGING_ENABLED_EVENTS() true
#define LOGGING_ENABLED_CONSOLE() true
#define LOGGING_ENABLED_FILE() true

#endif

#if defined(CONFIGURATION_RELEASE)

#define LOG_VERBOSITY() LogVerbosity::Warn
#define LOGGING_ENABLED_EVENTS() false
#define LOGGING_ENABLED_CONSOLE() true
#define LOGGING_ENABLED_FILE() true

#endif


