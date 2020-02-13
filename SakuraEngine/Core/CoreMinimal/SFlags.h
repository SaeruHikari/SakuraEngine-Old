/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Flags of Sakura Engine
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-08 14:36:44
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 16:49:22
 */
#pragma once

namespace Sakura::flags
{
    enum BuildVar
    {
        DEBUG_EDITOR = 1,
        DEBUG_GAME = 2,
        DEBUG_GAME_AND_EDITOR = 3,
        RELEASE = 4
    };
    enum TargetPlatform
    {
        Linux,
        Win32,
        OS_X
    };
    
}