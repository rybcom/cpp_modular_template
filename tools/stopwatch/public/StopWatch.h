
#pragma once

#include <chrono>
#include <string>

class StopWatch 
{
public:

   using clock = std::chrono::steady_clock;
   using microseconds = std::chrono::microseconds;
   using milliseconds = std::chrono::milliseconds;
   using seconds = std::chrono::seconds;


   StopWatch();
   StopWatch(const StopWatch&);
   StopWatch& operator=(const StopWatch& rhs);

   uint64_t elapsed_us() const;
   uint64_t elapsed_ms() const;
   uint64_t elapsed_s() const;

   std::chrono::steady_clock::time_point Restart();

   void printTime(std::string const & text = "Split time : ");

private:
   clock::time_point mStart;
};

