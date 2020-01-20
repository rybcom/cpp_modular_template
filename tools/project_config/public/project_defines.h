#pragma once

#pragma region preprocessor defines


#if CONFIGURATION_DEBUG==true

#define USE_GUI() false
#define USE_USER_INPUT_EVENTS() true
#define PROFILING_MODE() false
#define TUNNING_MODE() true
#define SANITY_CHECK_ENABLED() true

#endif


#if CONFIGURATION_PROFILER==true

#define USE_GUI() true
#define USE_USER_INPUT_EVENTS() true
#define PROFILING_MODE() false
#define TUNNING_MODE() true
#define SANITY_CHECK_ENABLED() true

#endif


#if CONFIGURATION_RELEASE==true

#define USE_GUI() true
#define USE_USER_INPUT_EVENTS() true
#define PROFILING_MODE() false
#define TUNNING_MODE() true
#define SANITY_CHECK_ENABLED() true


#endif

#define UNIT_TESTING_MODE() false

#if UNIT_TESTING_MODE() == true && USE_GUI() == true 
#undef USE_GUI()
#define USE_GUI() false
#endif

#pragma endregion

#pragma region main 

#define  RUN_AS_UNIT_TESTING() (UNIT_TESTING_MODE() == true)
#define  RUN_WITH_GUI() (USE_GUI() == true)
#define  RUN_CONSOLE_MAIN() (RUN_WITH_GUI()==false && RUN_AS_UNIT_TESTING() == false)  
#pragma endregion


#pragma region misc


#define DEBUG_BREAKPOINT() __debugbreak()

#if PROFILING_MODE() == true

#include "ProfilingTools.h"

#define  SCOPE_PROFILE(name) ProfilerTimeWatch timer##__LINE__(name)
#define  FUNCTION_PROFILE()  SCOPE_PROFILE(__FUNCSIG__)
#define  BEGIN_PROFILER_SESSION(name) ProfilerInstrument::Get().BeginSession(name) 
#define  END_PROFILER_SESSION(name) ProfilerInstrument::Get().EndSession() 

#else

#define  SCOPE_PROFILE(name) 
#define  FUNCTION_PROFILE()
#define  BEGIN_PROFILER_SESSION(name)  
#define  END_PROFILER_SESSION(name)

#endif

#if SANITY_CHECK_ENABLED() == true
#define TESTCHECK(expression) expression
#else
#define TESTCHECK(expression) 
#endif

#pragma endregion

