#pragma once
#include <SakuraSTL.hpp>
#include <spdlog/spdlog.h>
#include "vfs/path.h"

namespace sakura
{
	struct Logger;
}
namespace sakura
{
	struct RuntimeCoreAPI Logger
	{
		using ELevel = spdlog::level::level_enum;
		struct desc
		{
			sakura::vfs::path output_path = sakura::vfs::path::invalid_path;
			bool async = false;
		};
		struct RuntimeCoreAPI Ref
		{
			Ref(spdlog::logger& ref);

			static Ref get(const char* name) noexcept;

			template<typename... Ts> void info(const char* to_log, Ts&&... params) noexcept;
			template<typename... Ts> void trace(const char* to_log, Ts&&... params) noexcept;
			template<typename... Ts> void debug(const char* to_log, Ts&&... params) noexcept;
			template<typename... Ts> void warn(const char* to_log, Ts&&... params) noexcept;
			template<typename... Ts> void error(const char* to_log, Ts&&... params) noexcept;
			template<typename... Ts> void critical(const char* to_log, Ts&&... params) noexcept;

			void set_level(ELevel level) noexcept;
		private:
			spdlog::logger& spdlogger;
		};
		static void set_log_level(ELevel level);
	};
	extern RuntimeCoreAPI sakura::Logger::Ref default_logger;
}

namespace sakura
{
	sakura::Logger::Ref RuntimeCoreAPI register_logger(const char* name, const Logger::desc& desc) noexcept;

	template<typename... Ts> void info(const char* to_log, Ts&&... params) noexcept;
	template<typename... Ts> void trace(const char* to_log, Ts&&... params) noexcept;
	template<typename... Ts> void debug(const char* to_log, Ts&&... params) noexcept;
	template<typename... Ts> void warn(const char* to_log, Ts&&... params) noexcept;
	template<typename... Ts> void error(const char* to_log, Ts&&... params) noexcept;
	template<typename... Ts> void critical(const char* to_log, Ts&&... params) noexcept;
}

namespace sakura
{
	template <typename ... Ts>
	void Logger::Ref::info(const char* to_log, Ts&&... params) noexcept
	{
		return spdlogger.info(to_log, std::forward<Ts>(params)...);
	}
	
	template <typename ... Ts>
	void Logger::Ref::trace(const char* to_log, Ts&&... params) noexcept
	{
		return spdlogger.trace(to_log, std::forward<Ts>(params)...);
	}

	template <typename ... Ts>
	void Logger::Ref::debug(const char* to_log, Ts&&... params) noexcept
	{
		return spdlogger.debug(to_log, std::forward<Ts>(params)...);
	}

	template <typename ... Ts>
	void Logger::Ref::warn(const char* to_log, Ts&&... params) noexcept
	{
		return spdlogger.warn(to_log, std::forward<Ts>(params)...);
	}

	template <typename ... Ts>
	void Logger::Ref::error(const char* to_log, Ts&&... params) noexcept
	{
		return spdlogger.error(to_log, std::forward<Ts>(params)...);
	}

	template <typename ... Ts>
	void Logger::Ref::critical(const char* to_log, Ts&&... params) noexcept
	{
		return spdlogger.critical(to_log, std::forward<Ts>(params)...);
	}

	template <typename ... Ts>
	void info(const char* to_log, Ts&&... params) noexcept
	{
		return default_logger.info(to_log, std::forward<Ts>(params)...);
	}
	
	template <typename ... Ts>
	void warn(const char* to_log, Ts&&... params) noexcept
	{
		return default_logger.warn(to_log, std::forward<Ts>(params)...);
	}

	template <typename ... Ts>
	void error(const char* to_log, Ts&&... params) noexcept
	{
		return default_logger.error(to_log, std::forward<Ts>(params)...);
	}

	template <typename ... Ts>
	void debug(const char* to_log, Ts&&... params) noexcept
	{
		return default_logger.debug(to_log, std::forward<Ts>(params)...);
	}

	template <typename ... Ts>
	void trace(const char* to_log, Ts&&... params) noexcept
	{
		return default_logger.trace(to_log, std::forward<Ts>(params)...);
	}

	template <typename ... Ts>
	void critical(const char* to_log, Ts&&... params) noexcept
	{
		return default_logger.critical(to_log, std::forward<Ts>(params)...);
	}
}
