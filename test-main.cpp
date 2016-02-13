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
#include <string>
#define _LOG
#include "logger.h"

void unrelated_function_that_logs_stuff() {
    static const char* const tag = "MG"; // Lack of imagination, sorry!
    LOG("This is the most basic log call. ",
        "It will set the tag as empty and the level as DEBUG.");

    LOGD(tag, "In this case, ", "the level is DEBUG again, but it allows you to specify a particular tag.");
    LOGI(tag, "Now we can chage the level to INFO using LOGI instead of LOGD");
    LOGT(tag, "Another one changed, this time the level will be TRACE");

    int a = 30;
    float b = 12;
    std::string blah = " blah blah blah ";
    LOG("Note that no manual conversion is needed, as long as the parameters can be streamed to an ostream, ", a, ", ", b, ", ", blah);

    LOG("If you try recompiling the sample but removing the definition of \"_LOG\" (up, right before including logger.h), ",
        "none of this first few calls will be logged! ", 
        "The macros only work if \"_LOG\" is defined ",
        "This is handy when you have several build configurations, ", 
        "and some of the should not waste precious time logging anything.");
    
    debug::Logger::get().log(debug::fLevel::INFO, tag, "However, explicit calls will work.");
    FORCE_LOGD(tag, "In the same way the FORCE_ macros will work too :)");

    FORCE_LOGI(tag, "The logger works more or less as the print function from python ",
        "but it doesn't concatenates anything between elements on the list of arguments. ",
        "If you want spaces, you need to add them yourself.");
}

int main() {
    using namespace debug;
    // Initialize logger, let's use the default configuration.
    Logger::init(Logger_Config{});

    unrelated_function_that_logs_stuff();

    // Destroy the logger.
    Logger::destroy();
}
