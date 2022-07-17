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

#define  TRACE_FUNCTION() \
LOG_TRACE_FUNCTION(); 

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



