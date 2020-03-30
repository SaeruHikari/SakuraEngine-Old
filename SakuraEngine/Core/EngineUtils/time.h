/*
 * @Author: your name
 * @Date: 2020-02-09 15:43:31
 * @LastEditTime : 2020-02-09 16:49:07
 * @LastEditors  : SaeruHikari
 * @Description: In User Settings Edit
 * @FilePath: /SakuraEngine/Core/EngineUtils/time.h
 */
#pragma once
#include "../Containers/SString.h"

namespace Sakura
{
    namespace now
    {
        int64 ms(void);
        int64 us(void);
        /**
         * @description: output current system time.
         * @param output format 
         * @return: "%Y-%m-%d %H:%M:%S" ==> 2018-08-08 08:08:08
         */
        sstring str(const char* fm = "%Y-%m-%d %H:%M:%S");
    } 

    class Timer
    {
    public:
        Timer(void)
        {
            _start = now::us();
        }
        sinline void restart(void)
        {
            _start = now::us();
        }
        sinline int64 us() const
        {
            return now::us() - _start;
        }
        sinline int64 ms() const
        {
            return this->us() / 1000;
        }
    private:
        int64 _start;
    };
}
