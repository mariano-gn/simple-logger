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

#ifndef _UTIL_H_
#define _UTIL_H_
#include <sstream>

namespace util {
    namespace flag {
        template<typename FlagT, typename BitsT,
            typename = typename std::enable_if<
            std::is_enum<typename std::decay<FlagT>::type>::value &&
            std::is_enum<typename std::decay<BitsT>::type>::value
        >::type>
            inline bool flag_on(FlagT flag, BitsT bits) {
            return (
                static_cast<typename std::underlying_type<BitsT>::type>(flag) &
                static_cast<typename std::underlying_type<BitsT>::type>(bits)
                ) > 0;
        }

        template<typename FlagT, typename BitsT>
        inline bool flag_off(FlagT flag, BitsT bits) {
            return !flag_on(flag, bits);
        }

        template<typename FlagT, typename BitsT,
            typename = typename std::enable_if<
            std::is_enum<typename std::decay<FlagT>::type>::value &&
            std::is_enum<typename std::decay<BitsT>::type>::value
        >::type>
            inline void set_flag_on(FlagT flag, BitsT& bits) {
            bits = static_cast<BitsT>(
                static_cast<typename std::underlying_type<BitsT>::type>(bits) |
                static_cast<typename std::underlying_type<BitsT>::type>(flag));
        }

        template<typename FlagT, typename BitsT,
            typename = typename std::enable_if<
            std::is_enum<typename std::decay<FlagT>::type>::value &&
            std::is_enum<typename std::decay<BitsT>::type>::value
        >::type>
            inline void set_flag_off(FlagT flag, BitsT& bits) {
            bits = static_cast<BitsT>(
                static_cast<typename std::underlying_type<BitsT>::type>(bits) &
                ~(static_cast<typename std::underlying_type<BitsT>::type>(flag)));
        }
    } // namespace flag

    namespace string {
        template<typename Head>
        inline void concat_ss(std::stringstream& ss, Head&& h) {
            ss << h;
        }

        template<typename Head, typename... Tail>
        inline void concat_ss(std::stringstream& ss, Head&& h, Tail&&... args) {
            ss << h;
            concat_ss(ss, std::forward<Tail>(args)...);
        }

        template<typename... Args>
        inline std::string concat(Args&& ...args) {
            std::stringstream ss;
            concat_ss(ss, std::forward<Args>(args)...);
            return ss.str();
        }
    } // namespace string
} // namespace util

#endif // _UTIL_H_