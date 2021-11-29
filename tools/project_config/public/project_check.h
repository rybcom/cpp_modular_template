#pragma once

#if defined(CONFIGURATION_DEBUG) && defined(CONFIGURATION_PROFILER)

#error "multiple configuration preprocessor CONFIGURATION_DEBUG && CONFIGURATION_PROFILER "

#endif

#if defined(CONFIGURATION_DEBUG) && defined(CONFIGURATION_RELEASE)

#error "multiple configuration preprocessor CONFIGURATION_DEBUG && CONFIGURATION_RELEASE "

#endif

#if defined(CONFIGURATION_PROFILER) && defined(CONFIGURATION_RELEASE)

#error "multiple configuration preprocessor CONFIGURATION_PROFILER && CONFIGURATION_RELEASE "

#endif


#if ((defined(CONFIGURATION_DEBUG)==false) && (defined(CONFIGURATION_PROFILER)==false) && (defined(CONFIGURATION_RELEASE)==false))

#error "no configuration preprocessor is set. Use one of [ CONFIGURATION_DEBUG,CONFIGURATION_PROFILER,CONFIGURATION_RELEASE ]"

#endif



#if defined(CONFIGURATION_DEBUG)
#define CONFIGURATION_NAME() "Debug"
#endif

#if defined(CONFIGURATION_PROFILER)
#define CONFIGURATION_NAME() "Profiler"
#endif

#if defined(CONFIGURATION_RELEASE)
#define CONFIGURATION_NAME() "Release"
#endif

