/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 17:19:33
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 18:10:20
 */
#pragma once
#include "../CoreMinimal/SDefination.h"

namespace Sakura
{
    /**
     * @description: convert float point number to string
     * @param float val, char* buf out 
     * @return: string length
     * @author: SaeruHikari
     */    
    size_t ftoa(float val, char* buf);
    
    /**
     * @description: convert double float point number to string
     * @param double val, char* buf out 
     * @return: string length
     * @author: SaeruHikari
     */
    size_t dtoa(double val, char* buf);

    /**
     * @description: convert uint32 number to hex string 
     * @param uint32 val, char* buf out 
     * @return: string length
     * @author: SaeruHikari
     */
    size_t u32toh(uint32 val, char* buf);

    /**
     * @description: convert uint64 to hex string 
     * @param uint64 val, char* buf out
     * @return: string length
     * @author: SaeruHikari
     */
    size_t u64toh(uint64 val, char* buf);

    /**
     * @description: convert uint32 number to ASCII string
     * @param uint32 val, char* buf out
     * @return: string length
     * @author: SaeruHikari
     */
    size_t u32toa(uint32 val, char* buf);

    /**
     * @description: convert uint64 number to ASCII string 
     * @param uint64 val, char* buf out
     * @return: string length
     * @author: SaeruHikari
     */    
    size_t u64toa(uint64 val, char* buf);


    /**
     * @description: convert int64 number to ASCII string 
     * @param int64 val, char* buf out
     * @return: string length
     * @author: SaeruHikari
     */    
    sinline size_t i32toa(int64 val, char* buf)
    {
        if(val >= 0) 
            return u32toa((uint32)val, buf);
        *buf = '-';
        return u32toa((uint32)(-val), buf + 1) + 1;
    }

    /**
     * @description: 
     * @param {type} 
     * @return: 
     * @author: SaeruHikari
     */
    sinline size_t i64toa(int64 val, char* buf)
    {
        if (val >= 0)
            return u64toa((uint64)val, buf);
        *buf = '-';
        return u64toa((uint64)-val, buf + 1) + 1;
    }
}