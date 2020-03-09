#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "Core/CoreMinimal/SFlags.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Sakura::log
{
    using spdlog::info;
    using spdlog::error;
    using spdlog::log;
    using spdlog::warn;
    using spdlog::get;

    inline auto regist_logger(const char* channel)
    {
        if(spdlog::get(channel) != nullptr)
        {
            spdlog::get(channel)->warn("Log:: Do not allocate logger with same name twice!");
            return spdlog::get(channel);
        }
        else
            return spdlog::stdout_color_mt(channel);    
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_info_l(const char* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            spdlog::get(logger)->info(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            spdlog::get(logger)->info(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_info_l(spdlog::logger* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            logger->info(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            logger->info(params...);
            return;
        }
    #endif
    }

    template<typename... Ts>
    void info_l(const char* logger, Ts... params)
    {
        spdlog::get(logger)->info(params...);
    }
    template<typename... Ts>
    void info_l(spdlog::logger* logger, Ts... params)
    {
         logger->info(params...);
    }
    template<typename... Ts>
    void warn_l(const char* logger, Ts... params)
    {
        spdlog::get(logger)->warn(params...);
    }
    template<typename... Ts>
    void warn_l(spdlog::logger* logger, Ts... params)
    {
         logger->warn(params...);
    }
    template<typename... Ts>
    void error_l(const char* logger, Ts... params)
    {
        spdlog::get(logger)->error(params...);
    }
    template<typename... Ts>
    void error_l(spdlog::logger* logger, Ts... params)
    {
         logger->error(params...);
    }


    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_warn_l(spdlog::logger* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            logger->warn(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            logger->warn(params...);
            return;
        }
    #endif
    }

    template<Sakura::flags::BuildVar buildVar = flags::BuildVar::DEBUG_GAME_AND_EDITOR,
     typename... Ts>
    void debug_error_l(spdlog::logger* logger, Ts... params)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            logger->error(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            logger->error(params...);
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
            spdlog::get(logger)->warn(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            spdlog::get(logger)->warn(params...);
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
            spdlog::get(logger)->error(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            spdlog::get(logger)->error(params...);
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
            spdlog::info(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
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
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            spdlog::error(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
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
        if constexpr ((buildVar && flags::BuildVar::DEBUG_EDITOR) != 0)
        {
            spdlog::warn(params...);
            return;
        }
    #endif
    #ifdef SAKURA_DEBUG_GAME
        if constexpr ((buildVar && flags::BuildVar::DEBUG_GAME) != 0)
        {
            spdlog::warn(params...);
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
    template<typename... Ts>\
    inline static void error(Ts... params)\
    {\
        Sakura::log::error_l(get_logger(), params...);\
    }\
    template<typename... Ts>\
    inline static void info(Ts... params)\
    {\
        Sakura::log::info_l(get_logger(), params...);\
    }\
    template<typename... Ts>\
    inline static void warn(Ts... params)\
    {\
        Sakura::log::warn_l(get_logger(), params...);\
    }\
public: