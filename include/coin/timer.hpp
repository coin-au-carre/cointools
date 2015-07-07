#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace coin {

namespace _impl_timer {

template<typename TimeT = std::chrono::milliseconds>
class Timer {
	using clock = std::chrono::high_resolution_clock; // using type alias C++11 
	std::chrono::time_point<clock> start = clock::now();
	std::string label;
	Timer(const Timer& timer) = delete;
public:
	Timer(const std::string& lbl = "") : label(lbl) {}
	~Timer() {
		auto duration = std::chrono::duration_cast<TimeT>(clock::now() - start);
		std::cout << label  << "[Timer] " << duration.count() << " #" << std::endl;
	}
};


class TimerWatch {
	using clock = std::chrono::high_resolution_clock;
	bool is_running() const { return stop_time_ == clock::time_point::min(); }
	clock::time_point end_time() const { return is_running() ? clock::now() : stop_time_; }
	clock::time_point begin_time_{clock::now()}, stop_time_{clock::time_point::min()};
public:
	void stop() { if (is_running()) stop_time_ = clock::now(); }
	clock::duration elapsed() const { return end_time() - begin_time_; }
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


} // namespace _impl_timer

using _impl_timer::Timer;
using _impl_timer::TimerWatch;
using _impl_timer::TimerFunc;

} // namespace coin
