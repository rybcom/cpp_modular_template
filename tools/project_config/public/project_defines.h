#pragma once

#pragma region main 

#define UNIT_TESTING_MODE() false
#define  RUN_AS_UNIT_TESTING() (UNIT_TESTING_MODE() == true)

#pragma endregion

#pragma region misc


#if DEBUGGER_ASSERTION_ENABLED()
#define DEBUG_BREAKPOINT() __debugbreak()
#else
#define DEBUG_BREAKPOINT()
#endif


#if PROFILING_MODE() == true

#include "ProfilingTools.h"

#define  SCOPE_PROFILE(name) ProfilerTimeWatch timer##__LINE__(name)
#define  FUNCTION_PROFILE()  SCOPE_PROFILE(__FUNCSIG__)
#define  BEGIN_PROFILER_SESSION(file) ProfilerInstrument::Get().BeginSession(file) 
#define  END_PROFILER_SESSION() ProfilerInstrument::Get().EndSession() 

#else

#define  SCOPE_PROFILE(name) 
#define  FUNCTION_PROFILE()
#define  BEGIN_PROFILER_SESSION(file)  
#define  END_PROFILER_SESSION()

#endif

#define  TRACE_AND_PROFILE_FUNCTION() \
LOG_TRACE_FUNCTION(); \
FUNCTION_PROFILE() \

#if SANITY_CHECK_ENABLED() == true
#define TESTCHECK(expression) expression
#else
#define TESTCHECK(expression) 
#endif

#pragma endregion

#include <cstdint>

#ifndef NOMINMAX
#define NOMINMAX
#endif



