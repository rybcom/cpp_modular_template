#pragma once

enum LogVerbosity
{
	Trace= 1,
	Debug,
	Info,
	Warn,
	Error,
	Critical
};


#if CONFIGURATION_DEBUG==true

#define LOG_VERBOSITY() LogVerbosity::Info
#define LOGGING_ENABLED_IG() true
#define LOGGING_ENABLED_DIAGNOSTIC() true
#define LOGGING_ENABLED_CONSOLE() true

#endif


#if CONFIGURATION_PROFILER==true

#define LOG_VERBOSITY() LogVerbosity::Info
#define LOGGING_ENABLED_IG() true
#define LOGGING_ENABLED_DIAGNOSTIC() true
#define LOGGING_ENABLED_CONSOLE() true

#endif


#if CONFIGURATION_RELEASE==true

#define LOG_VERBOSITY() LogVerbosity::Warn
#define LOGGING_ENABLED_IG() false
#define LOGGING_ENABLED_DIAGNOSTIC() false
#define LOGGING_ENABLED_CONSOLE() true

#endif

#if USE_DIAGNOSTIC_VIEWER()==false && LOGGING_ENABLED_DIAGNOSTIC()==true
#define LOGGING_ENABLED_DIAGNOSTIC() false
#endif

#if USE_IG()==false && LOGGING_ENABLED_IG()==true
#define LOGGING_ENABLED_IG() false
#endif

