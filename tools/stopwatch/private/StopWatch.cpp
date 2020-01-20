/* 
 * File:   StopWatch.cpp
 * Author: KjellKod
 * From: https://github.com/KjellKod/StopWatch
 * 
 * Created on 2014-02-07 
 */

#include "StopWatch.h"
#include <iostream>
#include <iomanip>
StopWatch::StopWatch() : mStart(clock::now()) {
   static_assert(std::chrono::steady_clock::is_steady, "Serious OS/C++ library issues. Steady clock is not steady");
   // FYI:  This would fail  static_assert(std::chrono::high_resolution_clock::is_steady(), "High Resolution Clock is NOT steady on CentOS?!");
}

StopWatch::StopWatch(const StopWatch& other): mStart(other.mStart) { 
}

/// @return StopWatch::StopWatch&  - assignment operator.
StopWatch& StopWatch::operator=(const StopWatch& rhs) {
      mStart = rhs.mStart;
      return *this;
}

/// @return the elapsed microseconds since start
uint64_t StopWatch::elapsed_us() const {
   return std::chrono::duration_cast<microseconds>(clock::now() - mStart).count();
}

/// @return the elapsed milliseconds since start
uint64_t StopWatch::elapsed_ms() const {
   return std::chrono::duration_cast<milliseconds>(clock::now() - mStart).count();
}

/// @return the elapsed seconds since start
uint64_t StopWatch::elapsed_s() const {
   return std::chrono::duration_cast<seconds>(clock::now() - mStart).count();
}
/**
 * Resets the start point
 * @return the updated start point
 */
std::chrono::steady_clock::time_point StopWatch::Restart() {
   mStart = clock::now();
   return mStart;
}

void StopWatch::printTime(std::string const & text)
{
	std::cout << text << " -- " << std::setw(10) << this->elapsed_ms() <<"ms\n";
}




