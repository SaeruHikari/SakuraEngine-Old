/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 16:32:13
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-08 16:26:18
 */
#pragma once
#include "Core/CoreMinimal/SFlags.h"
#include <memory>
#include <iostream>

namespace Sakura::log
{
    struct logger
    {

    };
    
    template<typename... Params>
    inline std::shared_ptr<logger> get(Params... params)
    {
        return std::make_shared<logger>();
    }
    template<typename... Params>
    inline void error(Params... params)
    {
        
    }
    template<typename... Params>
    inline void info(Params... params)
    {
        
    }
    template<typename... Params>
    inline void warn(Params... params)
    {
        
    }
    template<typename P>
    void __log_internal(P p){std::cout << p << std::endl;}
    template<typename P, typename... Ps>
    void __log_internal(P p, Ps... ps)
    {
        std::cout << p << std::endl;
        __log_internal(ps...);
    }
    template<typename... Params>
    inline void log(Params... params)
    {
        __log_internal(params...);
    }

    inline auto regist_logger(const char* channel)
    {
        auto res = std::make_shared<logger>();
        return res;
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_info_l(const char* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            log(params...);
            //spdlog::get(logger)->info(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            //spdlog::get(logger)->info(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_info_l(logger* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            log(params...);
            //logger->info(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            //logger->info(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_warn_l(logger* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            log(params...);
            //logger->warn(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            //logger->warn(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_error_l(logger* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            log(params...);
            //logger->error(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            //logger->error(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_warn_l(const char* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            log(params...);
            //spdlog::get(logger)->warn(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            //spdlog::get(logger)->warn(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_error_l(const char* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            //spdlog::get(logger)->error(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            //spdlog::get(logger)->error(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_info(Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            
            //spdlog::info(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            //spdlog::info(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
        typename... Ts>
    void debug_error(Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            log(params...);
            //spdlog::error(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            //spdlog::error(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
        typename... Ts>
    void debug_warn(Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            log(params...);
            //spdlog::warn(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            //spdlog::warn(params...);
            return;
        }
    #endif
    }
}

#define DECLARE_LOGGER(name) \
public:\
    inline static auto get_logger()\
    {\
        static auto logger = Sakura::log::regist_logger(name);\
        return logger.get();\
    }\
	template<Sakura::flags::BuildVar buildVar = Sakura::flags::BuildVar::DEBUG_GAME_AND_EDITOR,\
		typename... Ts>\
	inline static void debug_warn(Ts... params)\
	{\
		Sakura::log::debug_warn_l<buildVar>(get_logger(), params...);\
	}\
    template<Sakura::flags::BuildVar buildVar = Sakura::flags::BuildVar::DEBUG_GAME_AND_EDITOR,\
		typename... Ts>\
	inline static void debug_info(Ts... params)\
	{\
		Sakura::log::debug_info_l<buildVar>(get_logger(), params...);\
	}\
	template<Sakura::flags::BuildVar buildVar = Sakura::flags::BuildVar::DEBUG_GAME_AND_EDITOR,\
		typename... Ts>\
	inline static void debug_error(Ts... params)\
	{\
		Sakura::log::debug_error_l<buildVar>(get_logger(), params...);\
	}\
public:


