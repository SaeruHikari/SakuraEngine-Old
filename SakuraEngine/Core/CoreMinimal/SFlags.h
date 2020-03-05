/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Flags of Sakura Engine
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-08 14:36:44
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-05 17:16:15
 */
#pragma once

namespace Sakura::flags
{
    enum BuildVar
    {
        DEBUG_EDITOR = 1,
        DEBUG_GAME = 2,
        DEBUG_GAME_AND_EDITOR = 3,
        RELEASE = 0x111111111
    };

    template<Sakura::flags::BuildVar buildVar>
    inline bool BuildVarFix()
    {
#ifdef SAKURA_DEBUG_EDITOR
    if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        return true;
#endif
#ifdef SAKURA_DEBUG_GAME
    if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        return true;
#endif
    }

    enum TargetPlatform
    {
        Linux,
        Win32,
        OS_X
    };
    
    template<Sakura::flags::TargetPlatform target>
    inline bool TargetPlatformFix()
    {
#ifdef _Linux
        if constexpr (target == TargetPlatform::Linux)
            return true;
#elif defined(_WIN32)
        if constexpr (target == TargetPlatform::Win32)
            return true;
#else 
        return false;
#endif
        return false;
    }
}