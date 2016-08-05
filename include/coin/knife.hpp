#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include <unistd.h>
#include <ios>
#include <fstream>
#include <utility>

namespace coin {

//! Ignore unused parameter/variable
template<class T> void ignore_unused_parameter( const T& ) { }

//! Force explicit conversion on enum class underlying type
template<typename E>
constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type {
   return static_cast<typename std::underlying_type<E>::type>(e);
}


inline
double get_total_physical_memory() {
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return (pages * page_size / 1024) / 1024;
}


// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in GB.
// On failure, returns 0.0, 0.0
inline
std::pair<double, double> process_mem_usage() {
   using std::ios_base;
   using std::ifstream;
   using std::string;

   double vm_usage     = 0.0;
   double resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = (vsize / 1024.0) / 1024.0;
   resident_set = (rss * page_size_kb) / 1024;

   return {vm_usage, resident_set};
}


//! System pause
inline
void pause(const std::string& msg = "") {
    std::cout << msg << std::endl;
    std::cin.get();
}

namespace _detail {

inline
std::vector<std::string> &split(std::string s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

} // ns _detail

// split string according to a delimiter
inline
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    _detail::split(s, delim, elems);
    return elems;
}


} // ns coin

