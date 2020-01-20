
#pragma once

#include <memory>
#include <fmt/core.h>
#include <fmt/color.h>

#if __has_include(<imgui.h>)
#include <imgui.h>
#else
#define LOGGING_ENABLED_DIAGNOSTIC() false
#endif
//



//
//
//   LOG ALWAYS
//
//
//

#define TRACE_LOG_ALWAYS(log_msg,...)  	\
ImGui::logText(ImGui::LogCategory::Trace,fmt::format(std::string("[trace] ").append(std::string(log_msg)),__VA_ARGS__).c_str()); \
fmt::print(fmt::fg(fmt::color::gray),std::string(log_msg)+"\n",__VA_ARGS__); 

#define DEBUG_LOG_ALWAYS(log_msg,...)  	\
ImGui::logText(ImGui::LogCategory::Trace,fmt::format(std::string("[debug] ").append(std::string(log_msg)),__VA_ARGS__).c_str()); \
fmt::print(fmt::fg(fmt::color::white),std::string(log_msg)+"\n",__VA_ARGS__); 

#define INFO_LOG_ALWAYS(log_msg,...)  	\
ImGui::logText(ImGui::LogCategory::Info,fmt::format(std::string("[info] ").append(std::string(log_msg)),__VA_ARGS__).c_str()); \
fmt::print(fmt::fg(fmt::color::green),std::string(log_msg)+"\n",__VA_ARGS__); 

#define WARN_LOG_ALWAYS(log_msg,...)  	\
ImGui::logText(ImGui::LogCategory::Warn,fmt::format(std::string("[warn] ").append(std::string(log_msg)),__VA_ARGS__).c_str()); \
fmt::print(fmt::fg(fmt::color::yellow),std::string(log_msg)+"\n",__VA_ARGS__); 

#define ERROR_LOG_ALWAYS(log_msg,...)  	\
ImGui::logText(ImGui::LogCategory::Error,fmt::format(std::string("[error] ").append(std::string(log_msg)),__VA_ARGS__).c_str()); \
fmt::print(fmt::fg(fmt::color::red),std::string(log_msg)+"\n",__VA_ARGS__); 

#define CRITICAL_LOG_ALWAYS(log_msg,...)  	\
ImGui::logText(ImGui::LogCategory::Error,fmt::format(std::string("[critical] ").append(std::string(log_msg)),__VA_ARGS__).c_str()); \
fmt::print(fmt::fg(fmt::color::red),std::string(log_msg)+"\n",__VA_ARGS__); 

//
//
//   LOG 
//
//

#define CONSOLE_LOG(log_msg,...) \
fmt::print(fmt::fg(fmt::color::white), std::string(log_msg) + "\n", __VA_ARGS__); 

#define IS_VERBOSITY_ACCEPET(verbosity) (LOG_VERBOSITY()<=LogVerbosity::##verbosity) 


#define TRACE_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_DIAGNOSTIC() &&  IS_VERBOSITY_ACCEPET(Trace)){ImGui::logText(ImGui::LogCategory::Trace,fmt::format(std::string("[trace] ").append(std::string(log_msg)),__VA_ARGS__).c_str());} \
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Trace))   {fmt::print(fmt::fg(fmt::color::gray),std::string(log_msg)+"\n",__VA_ARGS__);} \


#define DEBUG_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_DIAGNOSTIC() && IS_VERBOSITY_ACCEPET(Debug))	   {ImGui::logText(ImGui::LogCategory::Trace,fmt::format(std::string("[debug] ").append(std::string(log_msg)),__VA_ARGS__).c_str());} \
if constexpr(LOGGING_ENABLED_CONSOLE() &&  IS_VERBOSITY_ACCEPET(Debug))   {fmt::print(fmt::fg(fmt::color::white),std::string(log_msg)+"\n",__VA_ARGS__);} \


#define INFO_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_DIAGNOSTIC() && IS_VERBOSITY_ACCEPET(Info)) {ImGui::logText(ImGui::LogCategory::Info,fmt::format(std::string("[info] ").append(std::string(log_msg)),__VA_ARGS__).c_str());} \
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Info))    {fmt::print(fmt::fg(fmt::color::green),std::string(log_msg)+"\n",__VA_ARGS__);} \

#define WARN_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_DIAGNOSTIC() && IS_VERBOSITY_ACCEPET(Warn)) {ImGui::logText(ImGui::LogCategory::Warn,fmt::format(std::string("[warn] ").append(std::string(log_msg)),__VA_ARGS__).c_str());} \
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Warn))    {fmt::print(fmt::fg(fmt::color::yellow),std::string(log_msg)+"\n",__VA_ARGS__);} \

#define ERROR_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_DIAGNOSTIC() && IS_VERBOSITY_ACCEPET(Error)){ImGui::logText(ImGui::LogCategory::Error,fmt::format(std::string("[error] ").append(std::string(log_msg)),__VA_ARGS__).c_str());} \
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Error))   {fmt::print(fmt::fg(fmt::color::red),std::string(log_msg)+"\n",__VA_ARGS__);}  \

#define CRITICAL_LOG(log_msg,...)  	\
if constexpr(LOGGING_ENABLED_DIAGNOSTIC() && IS_VERBOSITY_ACCEPET(Critical)){ImGui::logText(ImGui::LogCategory::Error,fmt::format(std::string("[critical] ").append(std::string(log_msg)),__VA_ARGS__).c_str());} \
if constexpr(LOGGING_ENABLED_CONSOLE() && IS_VERBOSITY_ACCEPET(Critical))   {fmt::print(fmt::fg(fmt::color::red),std::string(log_msg)+"\n",__VA_ARGS__);} \


#define  LOG_TRACE_FUNCTION()  TRACE_LOG(__FUNCSIG__)


