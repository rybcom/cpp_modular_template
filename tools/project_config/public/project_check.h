#pragma once

#ifndef CONFIGURATION_DEBUG

#error "please set preprocessor define CONFIGURATION_DEBUG=true or CONFIGURATION_DEBUG=false "

#endif

#ifndef CONFIGURATION_PROFILER

#error "please set preprocessor define CONFIGURATION_PROFILER=true or CONFIGURATION_PROFILER=false "

#endif

#ifndef CONFIGURATION_RELEASE

#error "please set preprocessor define CONFIGURATION_RELEASE=true or CONFIGURATION_RELEASE=false "

#endif

#if CONFIGURATION_DEBUG && CONFIGURATION_PROFILER

#error "only one configuration can be set"

#endif

#if CONFIGURATION_DEBUG && CONFIGURATION_RELEASE

#error "only one configuration can be set"

#endif

#if CONFIGURATION_PROFILER && CONFIGURATION_RELEASE

#error "only one configuration can be set"

#endif

#if CONFIGURATION_DEBUG==false&& CONFIGURATION_PROFILER==false && CONFIGURATION_RELEASE==false

#error "set configuration preprocessor define CONFIGURATION_DEBUG==true ..

#endif
