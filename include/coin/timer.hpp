#pragma once

#include <chrono>
#include <iostream>
#include <string>

#include "logger.hpp"

namespace coin {

namespace _detail_timer {

template<typename TimeT>
std::string suffix_duration();

// explicit full template specializations
// the inlines are just a trick to tell the compiler not to define them multiple times
template<> inline std::string suffix_duration<std::chrono::hours>()        { return "hours"; }
template<> inline std::string suffix_duration<std::chrono::minutes>()      { return "min"; }
template<> inline std::string suffix_duration<std::chrono::seconds>()      { return "s"; }
template<> inline std::string suffix_duration<std::chrono::milliseconds>() { return "ms"; }
template<> inline std::string suffix_duration<std::chrono::microseconds>() { return "\xC2\xB5s"; }
template<> inline std::string suffix_duration<std::chrono::nanoseconds>()  { return "ns"; }


template<LogLevel Level=coin::LogLevel::log_debug, typename TimeT = std::chrono::milliseconds>
class Timer {
    using clock = std::chrono::high_resolution_clock; 
    clock::time_point begin_time_{clock::now()};
    std::string label;
public:
    Timer(const std::string& lbl = "") : label(lbl) {}
    void end() const {
        auto duration = std::chrono::duration_cast<TimeT>(clock::now() - begin_time_);
        coin::_detail::Log<Level>(std::cout) << "[Timer] " 
            << label  << " : " << duration.count() << " " << suffix_duration<TimeT>() <<"\n";
    }
};


template<LogLevel Level=coin::LogLevel::log_debug, typename TimeT = std::chrono::milliseconds>
class TimerScope {
    using clock = std::chrono::high_resolution_clock; 
    clock::time_point begin_time_{clock::now()};
    std::string label;
    TimerScope(const TimerScope& timer) = delete;
public:
    TimerScope(const std::string& lbl = "") : label(lbl) {}
    ~TimerScope() {
        auto duration = std::chrono::duration_cast<TimeT>(clock::now() - begin_time_);
        coin::_detail::Log<Level>(std::cout) << "[Timer] " 
            << label  << " : " << duration.count() << " " << suffix_duration<TimeT>() <<"\n";
    }
};


//! Return the time elapsed for a specific function passed in argument
/*
    Usage : std::cout << TimerFunc<std::chrono::microseconds>::exec<bool(unsigned int)>(isPrime, 7);
*/
template<typename TimeT = std::chrono::milliseconds>
struct TimerFunc {
    using clock = std::chrono::high_resolution_clock;
    template<typename F, typename ...Args>
    static typename TimeT::rep exec(F func, Args&&... args) {
        auto start = clock::now();
        func(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast<TimeT>(clock::now() - start);
        return duration.count();
    }
};

} // ns _detail_timer

using _detail_timer::TimerScope;
using _detail_timer::Timer;
using _detail_timer::TimerFunc;

} // ns mq


