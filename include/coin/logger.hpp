#pragma once

#include <stdexcept>
#include <functional>
#include <memory>
#include <iosfwd>
#include <fstream>
#include <string>
#include <array>
#include <unordered_map>

#include "date.hpp"
#include "knife.hpp"

#include "pretty_print.hpp"

// Use -DDISABLE_LOG flag if you want to disable LOG. Should not be a problem for performance.
// Not GCC 4.9 compliant  without forcing this flag.

#ifdef DISABLE_LOG

# define DISABLE_LOG_FLAG 1 

static std::stringstream k_black_hole{}; // wow darkness

# define LOGERROR                 std::cerr 
# define LOGWARNING               std::cerr 
# define LOGNOTICE                std::cout 
# define LOGINFO                  k_black_hole
# define LOGDEBUG                 k_black_hole
# define LOGCRAZY                 k_black_hole
# define LOGERROR_FILE(LogFile)   std::cerr
# define LOGWARNING_FILE(LogFile) std::cerr
# define LOGNOTICE_FILE(LogFile)  std::cout
# define LOGINFO_FILE(LogFile)    k_black_hole
# define LOGDEBUG_FILE(LogFile)   k_black_hole
# define LOGCRAZY_FILE(LogFile)   k_black_hole

#else

# define DISABLE_LOG_FLAG 0

# define LOGERROR                 coin::LogError(std::cerr, __FILE__, __FUNCTION__, __LINE__)
# define LOGWARNING               coin::LogWarning(std::cout, __FILE__, __FUNCTION__, __LINE__)
# define LOGNOTICE                coin::LogNotice(std::cout, __FILE__, __FUNCTION__, __LINE__)
# define LOGINFO                  coin::LogInfo(std::cout, __FILE__, __FUNCTION__, __LINE__)
# define LOGDEBUG                 coin::LogDebug(std::cout, __FILE__, __FUNCTION__, __LINE__)
# define LOGCRAZY                 coin::LogCrazy(std::cout, __FILE__, __FUNCTION__, __LINE__)
# define LOGERROR_FILE(LogFile)   coin::LogError(LogFile, __FILE__, __FUNCTION__, __LINE__)
# define LOGWARNING_FILE(LogFile) coin::LogWarning(LogFile, __FILE__, __FUNCTION__, __LINE__)
# define LOGNOTICE_FILE(LogFile)  coin::LogNotice(LogFile, __FILE__, __FUNCTION__, __LINE__)
# define LOGINFO_FILE(LogFile)    coin::LogInfo(LogFile, __FILE__, __FUNCTION__, __LINE__)
# define LOGDEBUG_FILE(LogFile)   coin::LogDebug(LogFile, __FILE__, __FUNCTION__, __LINE__)
# define LOGCRAZY_FILE(LogFile)   coin::LogCrazy(LogFile, __FILE__, __FUNCTION__, __LINE__)

#endif



namespace coin {

enum class LogLevel { log_none = 0, log_error = 1, log_warning = 2, log_notice = 3, log_info = 4, log_debug = 5, log_crazy = 6 };

const std::string fc_white{"\033[0m"};
const std::string fc_green{"\033[1;32m"};
const std::string fc_red{"\033[1;31m"};
const std::string fc_magenta{"\033[1;95m"};
const std::string fc_yellow{"\033[1;33m"};
const std::string fc_cyan{"\033[36m"};

struct LogParameters {
    LogParameters() = default;
    LogLevel     global_level   {LogLevel::log_notice};
    bool         timestamp      {false};
    std::string  path           {"log_coin.log"};
    std::string  prefix         {""};
    std::array<std::string, to_integral(LogLevel::log_crazy) + 1> label {
        { " "
        , fc_red     + " [error]" + fc_white
        , fc_magenta + " [warn] " + fc_white
        , fc_yellow  + " [note] " + fc_white
        , fc_green   + " [info] " + fc_white
        , fc_cyan    + " [debug]" + fc_white
        , fc_white   + " [crazy]" + fc_white }
    };
    std::unordered_map<std::string, LogLevel> mapping {
        {"crazy", LogLevel::log_crazy},
        {"debug", LogLevel::log_debug},
        {"warning", LogLevel::log_warning},
        {"info", LogLevel::log_info},
        {"notice", LogLevel::log_notice},
        {"warning", LogLevel::log_warning},
        {"error", LogLevel::log_error},
        {"none", LogLevel::log_none}
    };

    static LogParameters& instance() {
        static LogParameters parameters;
        return parameters;
    }
    
    LogParameters(const LogParameters&)  = delete;
    void operator=(const LogParameters&) = delete;
};

#if !DISABLE_LOG_FLAG

namespace _detail {

#ifdef NDEBUG
    #if defined(__clang__)
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wunused-parameter"
    #elif defined(__GNUC__) || defined(__GNUG__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunused-parameter"
    #endif
#endif


inline 
std::chrono::system_clock::time_point get_current_time_with_offset() {
    const int hours_offset = 2; // modify this when needed
    auto offset = std::chrono::duration<int,std::ratio<60*60>>{hours_offset};
    auto now = std::chrono::system_clock::now();
    return now + offset;
}


template<LogLevel Level>
class Log {
public:
    Log(std::ostream& os, const char *file = "", const char *function = "", int line = 0) 
        : parameters_{LogParameters::instance()}
        , is_loggable_(Level <= parameters_.global_level)
        , save_in_file_(false)
        , file_(file)
        , function_(function)
        , line_(line) 
        , ptr_os_(&os, [](std::ostream*){}) {
            if(is_loggable_) {
                (*ptr_os_) << parameters_.prefix << parameters_.label[to_integral(Level)];
            }
            if(is_loggable_ && parameters_.timestamp) {
                auto now = get_current_time_with_offset();
                using date::operator<<;
                (*ptr_os_) << fc_cyan + "[" << now << "] " + fc_white;
            }
        } 
        
    explicit Log(const std::string& logfile_path, const char *file = "", const char *function = "", int line = 0) 
        : parameters_{LogParameters::instance()}
        , is_loggable_(Level <= parameters_.global_level)
        , save_in_file_(true)
        , file_(file)
        , function_(function)
        , line_(line)
        , ptr_os_(new std::ofstream(logfile_path), std::default_delete<std::ostream>()) {
            if (!dynamic_cast<std::ofstream&>(*ptr_os_).is_open()) {
                throw std::ios_base::failure("Cannot open file " + logfile_path);
            }
            if(parameters_.timestamp) {
                auto now = get_current_time_with_offset();
                using date::operator<<;
                (*ptr_os_) << fc_cyan + "[" << now << "] " + fc_white;
            }
        }

    ~Log() { if (save_in_file_) { dynamic_cast<std::ofstream&>(*ptr_os_).close(); }}

    template<typename T>
    Log& operator<<(T msg) {
        if(is_loggable_) { 
            (*ptr_os_) << msg; 
        }
        return *this;
    }

private:
    LogParameters& parameters_; // instanceLogParameters()
    bool is_loggable_;
    bool save_in_file_;
    const char* file_;
    const char* function_;
    int line_;
    std::unique_ptr<std::ostream, std::function<void(std::ostream*)>> ptr_os_;
};

#ifdef NDEBUG
    #if defined(__clang__)
        #pragma clang diagnostic pop
    #elif defined(__GNUC__) || defined(__GNUG__)
        #pragma GCC diagnostic pop
    #endif
#endif


template<>
inline
Log<LogLevel::log_error>::Log(std::ostream& os, const char *file, const char *function, int line) 
    : parameters_{LogParameters::instance()}
    , is_loggable_(LogLevel::log_error <= parameters_.global_level)
    , save_in_file_(false)
    , file_(file)
    , function_(function)
    , line_(line) 
    , ptr_os_(&os, [](std::ostream*){}) {
        auto now = get_current_time_with_offset();
        using date::operator<<;
        (*ptr_os_) << fc_red + parameters_.prefix << parameters_.label[to_integral(LogLevel::log_error)] << fc_red + "[" << now << "] " + fc_white 
            << "in (" << file_ << "->" << function_ << ":" << line_ << ") " + fc_red;
    }

template<>
inline
Log<LogLevel::log_warning>::Log(std::ostream& os, const char *file, const char *function, int line) 
    : parameters_{LogParameters::instance()}
    , is_loggable_(LogLevel::log_warning <= parameters_.global_level)
    , save_in_file_(false)
    , file_(file)
    , function_(function)
    , line_(line) 
    , ptr_os_(&os, [](std::ostream*){}) {
        auto now = get_current_time_with_offset();
        using date::operator<<;
        (*ptr_os_) << fc_yellow + parameters_.prefix << parameters_.label[to_integral(LogLevel::log_warning)] << fc_yellow + " [" << now << "] " + fc_magenta 
            << "in (" << file_ << "->" << function_ << ":" << line_ << ") " + fc_yellow;
    }

template class Log<LogLevel::log_error>;
template class Log<LogLevel::log_warning>;
template class Log<LogLevel::log_notice>;
template class Log<LogLevel::log_info>;
template class Log<LogLevel::log_debug>;
template class Log<LogLevel::log_crazy>;

} // ns _detail

using LogError   = _detail::Log<LogLevel::log_error>;
using LogWarning = _detail::Log<LogLevel::log_warning>;
using LogNotice  = _detail::Log<LogLevel::log_notice>;
using LogInfo    = _detail::Log<LogLevel::log_info>;
using LogDebug   = _detail::Log<LogLevel::log_debug>;
using LogCrazy   = _detail::Log<LogLevel::log_crazy>;

#endif

} // ns coin

