#pragma once

#include "project_check.h"

#if defined(CONFIGURATION_DEBUG)

#define USE_GUI() true
#define DEVEL_MODE_ENABLED() true
#define SANITY_CHECK_ENABLED() true
#define DEBUGGER_ASSERTION_ENABLED() false

#endif

#if defined(CONFIGURATION_PROFILER)

#define USE_GUI() true
#define DEVEL_MODE_ENABLED() true
#define SANITY_CHECK_ENABLED() true
#define DEBUGGER_ASSERTION_ENABLED() false

#endif

#if defined(CONFIGURATION_RELEASE)

#define USE_GUI() false
#define DEVEL_MODE_ENABLED() true
#define SANITY_CHECK_ENABLED() true
#define DEBUGGER_ASSERTION_ENABLED() false

#endif


