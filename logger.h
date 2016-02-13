/**
Copyright (c) 2016 Mariano Gonzalez

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _LOGGER_H_
#define _LOGGER_H_
#include "patterns/singleton.h"
#include "util.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#define FORCE_LOGI(tag, ...) debug::Logger::get().ts_log(debug::fLevel::INFO, tag, __VA_ARGS__)
#define FORCE_LOGD(tag, ...) debug::Logger::get().ts_log(debug::fLevel::DEBUG, tag, __VA_ARGS__)
#define FORCE_LOGT(tag, ...) debug::Logger::get().ts_log(debug::fLevel::TRACE, tag, __VA_ARGS__)

#if defined(_LOG)
#   define LOG(...) LOGI("", __VA_ARGS__)
#   define LOGI(tag, ...) FORCE_LOGI(tag, __VA_ARGS__)
#   define LOGD(tag, ...) FORCE_LOGD(tag, __VA_ARGS__)
#   define LOGT(tag, ...) FORCE_LOGT(tag, __VA_ARGS__)
#else
#   define LOG(...)
#   define LOGI(tag, ...)
#   define LOGD(tag, ...)
#   define LOGT(tag, ...)
#endif

namespace debug {
    // Level of logging (the bigger the enum, the more it's supposed to print)
    enum fLevel : short {
        INFO = 0,
        DEBUG,
        TRACE,
    };

    // The types of logging (you could potentially enable more of one log output)
    enum fType : short {
        FILESYSTEM = 0x1 << 0,
        CONSOLE = 0x1 << 1
    };


    inline const char* toString(fLevel l) {
        static const char* const strs[] = { "INFO", "DEBUG", "TRACE" };
        return strs[l];
    }

    struct Logger_Config {
        // Both logging types enabled by default
        fType logger_types = static_cast<fType>(fType::CONSOLE | fType::FILESYSTEM);
        // We'll dump the log files in the working directory 
        // (note that on VS and cmake, this is the project folder, not the location of the exe).
        std::string path_to_log_folder = ".";
        // This makes a different file each time the app run, because a timestamp will be added to the filename.
        // The other option overrides the log file.
        bool use_timestamps = true;
        // Log all by default.
        fLevel level = fLevel::TRACE;
    };

    class Logger : public patterns::Singleton<Logger> {
    public:
        template<typename... Type>
        void ts_log(fLevel level, const char* tag, Type&&... obj) {
            if (level <= m_active_level) {
                std::lock_guard<std::mutex> lock(m_mutex);
                log_impl(level, tag, std::forward<Type>(obj)...);
            }
        }

        template<typename... Type>
        void log(fLevel level, const char* tag, Type&&... obj) {
            if (level <= m_active_level) {
                log_impl(level, tag, std::forward<Type>(obj)...);
            }
        }

    protected:
        explicit Logger(const Logger_Config& config);
        friend class patterns::Singleton<Logger>;

    private:
        fType m_type_flags = fType::CONSOLE;
        fLevel m_active_level = fLevel::TRACE;
        std::unique_ptr<std::ostream> m_pout{ nullptr };
        std::mutex m_mutex;

        template<typename... Type>
        void log_impl(fLevel level, const char* tag, Type&&... obj) {
            if (util::flag::flag_on(fType::FILESYSTEM, m_type_flags)) {
                SPL_ASSERT(m_pout, "Trying to log to the filesystem without a valid ostream");
                log_helper(*m_pout, level, tag, std::forward<Type>(obj)...);
            }
            if (util::flag::flag_on(fType::CONSOLE, m_type_flags)) {
                log_helper(std::cout, level, tag, std::forward<Type>(obj)...);
            }
        }

        // As long as the objects passed to log() method support streaming operators, this will work.
        // A good upgrade to this implementation would be to use SFINAE to prevent the validity of a call
        // given an object that doesn't support the operator.
        template<typename Type>
        static void log_helper(std::ostream& out, fLevel level, const char* tag, Type&& obj) {
            out << toString(level) << " | ";
            out << "TID:" << std::this_thread::get_id() << " | ";
            out << tag << " | ";
            out << obj << "\n";
        }
        template<typename... Type>
        static void log_helper(std::ostream& out, fLevel level, const char* tag, Type&&... obj) {
            log_helper(out, level, tag, util::string::concat(obj...));
        }
    };
}

#endif // _LOGGER_H_