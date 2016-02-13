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

#include "logger.h"
#include <ctime>
#include <fstream>

using namespace debug;
using namespace util;

Logger::Logger(const Logger_Config& config) {
    m_type_flags = config.logger_types;
    // Many better controls could be made to the path and how it's loaded, but hey, one project at a time.
    if (flag::flag_on(fType::FILESYSTEM, m_type_flags) && !config.path_to_log_folder.empty()) {
        auto log_file = config.use_timestamps ?
            string::concat(config.path_to_log_folder, "/log-", std::time(nullptr), ".txt") :
            string::concat(config.path_to_log_folder, "/log.txt");

        m_pout.reset(new std::ofstream(log_file, std::ios_base::trunc));
        if (!dynamic_cast<std::ofstream*>(m_pout.get())->is_open()) {
            // Creating the file pointer failed! Bummer :(
            flag::set_flag_off(fType::FILESYSTEM, m_type_flags);
            m_pout.reset();
        }
    }
    m_active_level = config.level;
}
