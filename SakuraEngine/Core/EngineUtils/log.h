/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 16:32:13
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 17:34:35
 */
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "Core/CoreMinimal/SFlags.h"

namespace Sakura::log
{
    using spdlog::info;
    using spdlog::error;
    using spdlog::log;
    using spdlog::warn;

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_info(Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar || flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            spdlog::info(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar || flags::BuildVar::DEBUG_GAME) != 0)
        {
            spdlog::info(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
        typename... Ts>
    void debug_error(Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar || flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            spdlog::error(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar || flags::BuildVar::DEBUG_GAME) != 0)
        {
            spdlog::error(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
        typename... Ts>
    void debug_warn(Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar || flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            spdlog::warn(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar || flags::BuildVar::DEBUG_GAME) != 0)
        {
            spdlog::warn(params...);
            return;
        }
    #endif
    }
}