#pragma once

#include <stdexcept>
#include <exception>

#include "preprocessor.hpp"

#ifndef NDEBUG
#define GSL_THROW_ON_CONTRACT_VIOLATION
#endif

namespace coin {

struct fail_fast : public std::runtime_error {
    explicit fail_fast(const std::string& pre_msg, const std::string& msg) : std::runtime_error(pre_msg + msg) {}   
};


struct connect_error : public std::runtime_error {
    explicit connect_error(const std::string& msg) : std::runtime_error(msg) {}
};

}

// #ifdef GSL_THROW_ON_CONTRACT_VIOLATION
#define Precondition(cond, msg)  if (!(cond)) \
    throw coin::fail_fast("GSL: Precondition failure at " __FILE__ ":" COIN_STRINGIZE(__LINE__)" ", msg);

#define Postcondition(cond, msg) if (!(cond)) \
    throw coin::fail_fast("GSL: Postcondition failure at " __FILE__ ":" COIN_STRINGIZE(__LINE__)" ", msg);
// #else
// #define Precondition(cond, msg)  if (!(cond)) std::terminate();
// #define Postcondition(cond, msg) if (!(cond)) std::terminate();

// #endif

