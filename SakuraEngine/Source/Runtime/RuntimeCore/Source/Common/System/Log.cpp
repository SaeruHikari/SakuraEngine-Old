#include <filesystem>
#include <System/Log.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>
#include <System/vfs/fs_dev_local/adapter_dev_local.h>
#include <spdlog/sinks/basic_file_sink.h>

sakura::Logger::Ref::Ref(spdlog::logger& ref)
	:spdlogger(ref)
{
	
}

void sakura::Logger::Ref::set_level(sakura::Logger::ELevel level) noexcept
{
    spdlogger.set_level(level);
}

namespace sakura
{
    sakura::Logger::Ref default_logger
        = sakura::register_logger("default_logger", { sakura::vfs::path::invalid_path, false });

    void Logger::set_log_level(ELevel level)
    {
        default_logger.set_level(level);
    }
}
sakura::Logger::Ref sakura::register_logger(const char* channel, const Logger::desc& desc) noexcept
{
    if (spdlog::get(channel) != nullptr)
    {
        spdlog::get(channel)->warn("Log:: Do not allocate logger with same name twice!");
        return sakura::Logger::Ref(*spdlog::get(channel));
    }
    else
    {
	    if(desc.output_path == sakura::vfs::path::invalid_path)
	    {
            return desc.async
	    		? sakura::Logger::Ref(*spdlog::stdout_color_mt<spdlog::async_factory>(channel))
	    		: sakura::Logger::Ref(*spdlog::stdout_color_mt(channel));
	    }
        else
        {
            assert(desc.output_path.adapter_name() == sakura::vfs::dev_local::adapter_dev_local::get()->get_symbol() && "path is not a dev_local adapter path!");
            sakura::u8string result = vfs::dev_local::adapter_dev_local::get()->project_dir;
            auto path = result.append(desc.output_path.u8string().c_str()).c_str();
            return desc.async
                ? sakura::Logger::Ref(*spdlog::basic_logger_mt<spdlog::async_factory>(channel, (const char*)path))
                : sakura::Logger::Ref(*spdlog::basic_logger_mt(channel, (const char*)path));
        }
    }
}

sakura::Logger::Ref sakura::Logger::Ref::get(const char* name) noexcept
{
    return sakura::Logger::Ref(*spdlog::get(name));
}
