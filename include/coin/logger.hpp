#pragma once

#include <stdexcept>
#include <functional>
#include <memory>
#include <iosfwd>
#include <fstream>
#include <string>
#include <array>

namespace coin {
	
enum log_level { log_none = 0, log_error = 1, log_warning = 2, log_info = 3, log_debug = 4 };
	
#ifndef NDEBUG
	static log_level    log_global_level = log_warning;
	static log_level    log_specific_level = log_none;
	static std::string  log_path = "log_coin.log";
	static std::string  log_prefix = ">> ";
	static std::array<std::string, log_debug + 1> log_level_mapping{{" ", "[Error] ", "[Warning] ", "[Info] ", "[Debug] " }};

	template<log_level Level>
	class Log {
	public:
		explicit Log(std::ostream& os) 
			: is_loggable_(Level <= log_global_level)
			, save_in_file_(false)
			, ptr_os_(&os, [](std::ostream*){}) {
				if(is_loggable_) {
					(*ptr_os_) << log_prefix << log_level_mapping[Level];
				}
			}
			
		explicit Log(const std::string& filepath) 
			: is_loggable_(Level <= log_global_level)
			, save_in_file_(true)
			, ptr_os_(new std::ofstream(filepath), std::default_delete<std::ostream>()) {
				if (!dynamic_cast<std::ofstream&>(*ptr_os_).is_open()) {
					throw std::ios_base::failure("Cannot open file " + filepath);
				}
			}
			
		~Log() { if (save_in_file_) { dynamic_cast<std::ofstream&>(*ptr_os_).close(); }}
		
		template<typename T>
		Log& operator<<(T t) {
			if(is_loggable_) {
				(*ptr_os_) << t;
			}
			return *this;
		}
	
	private:
		bool is_loggable_;
		bool save_in_file_;
		std::unique_ptr<std::ostream, std::function<void(std::ostream*)>> ptr_os_;
	};
#else
	static std::string  log_path = nullptr;
	template<log_level Level> struct Log {
		Log(std::ostream& os) {}
		Log(const std::string& filepath) {}
		template<typename T> Log& operator<<(T t) { return *this; } 
	};
#endif

void setLogLevel(log_level lvl);
void setLogPath(const std::string& path);
void setLogPrefix(const std::string& prefix);
void setLogMapping(const std::array<std::string, log_debug + 1>& mapping);

Log<log_error>   logError();
Log<log_error>   logError(const std::string& file);
Log<log_warning> logWarning();
Log<log_warning> logWarning(const std::string& file);
Log<log_info>    logInfo();
Log<log_info>    logInfo(const std::string& file);
Log<log_debug>   logDebug();
Log<log_debug>   logDebug(const std::string& file);

} // namespace coin



// cpp

#include "logger.hpp"


#include <iostream>

namespace coin {



#ifndef NDEBUG	
	void setLogLevel(log_level lvl) { log_global_level = lvl; }
	void setLogPath(const std::string& path) { log_path = path; }
	void setLogPrefix(const std::string& prefix) { log_prefix = prefix; }
	void setLogMapping(const std::array<std::string, log_debug + 1>& mapping) { log_level_mapping = mapping; }
#else
	void setLogLevel(log_level lvl) {}
	void setLogPath(const std::string& path) {}
	void setLogPrefix(const std::string& prefix) {}
	void setLogMapping(const std::array<const std::array<const std::string, log_debug + 1>&) {}
#endif

	
Log<log_error>   logError()                          { return Log<log_error>(std::cout); }
Log<log_error>   logError(const std::string& file)   { return Log<log_error>(file); }
Log<log_warning> logWarning()                        { return Log<log_warning>(std::cout); }
Log<log_warning> logWarning(const std::string& file) { return Log<log_warning>(file); }
Log<log_info>    logInfo()                           { return Log<log_info>(std::cout); }
Log<log_info>    logInfo(const std::string& file)    { return Log<log_info>(file); }
Log<log_debug>   logDebug()                          { return Log<log_debug>(std::cout); }
Log<log_debug>   logDebug(const std::string& file)   { return Log<log_debug>(file); }

}

