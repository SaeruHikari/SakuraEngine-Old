/*
 * @Author: your name
 * @Date: 2020-07-30 14:06:44
 * @LastEditTime: 2020-08-19 18:19:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \SakuraEngineV3\SakuraEngine\Runtime\Core\private\Core.cpp
 */
#include <iomanip>
#include <RuntimeCore/RuntimeCore.h>
#include "System/vfs/fs_dev_local/adapter_dev_local.h"

using namespace sakura;

// Module
sakura::unordered_map<sakura::string, IModule::constructor_t> IModule::constructors_;
sakura::unordered_map<sakura::string, IModule*> IModule::modules_;
bool IModule::StartUp(const sakura::string& moduleName)
{
	if (modules_.find(moduleName) != modules_.end())
	{
		sakura::error("module {} StartUp Failed: An module with this name already StartUp!", moduleName);
		return false;
	}
	if (constructors_.find(moduleName) == constructors_.end())
	{
		sakura::error("module {} Register Not Found!", moduleName);
		return false;
	}
	modules_[moduleName] = constructors_[moduleName]();
	modules_[moduleName]->StartUp();
	return true;
}

bool IModule::Terminate(const sakura::string& moduleName)
{
	if (modules_.find(moduleName) == modules_.end())
	{
		sakura::error("module {} Terminate Failed: module not found!", moduleName);
		return false;
	}
	modules_[moduleName]->Terminate();
	delete modules_[moduleName];
	return true;
}

bool IModule::Registry::regist(const sakura::string& moduleName, const constructor_t& constructor)
{
	if (constructors_.find(moduleName) != constructors_.end())
	{
		sakura::error("module {} Registry Failed: An module with this name already registered!", moduleName);
		return false;
	}
	constructors_.insert_or_assign(moduleName, constructor);
	return true;
}


// DateTime
sakura::DateTime sakura::DateTime::now() noexcept
{
	auto now = time(0);
	return to_date_time(&now);
}

bool sakura::DateTime::operator<(const sakura::DateTime& other) const noexcept
{
	return std::tie(tm_year, tm_mon, tm_mday, tm_hour, tm_min, tm_sec) <
		std::tie(other.tm_year, other.tm_mon, other.tm_mday, other.tm_hour, other.tm_min, other.tm_sec);
}
bool sakura::DateTime::operator==(const sakura::DateTime& other) const noexcept
{
	return std::tie(tm_year, tm_mon, tm_mday, tm_hour, tm_min, tm_sec) ==
		std::tie(other.tm_year, other.tm_mon, other.tm_mday, other.tm_hour, other.tm_min, other.tm_sec);
}
bool sakura::DateTime::operator!=(const sakura::DateTime& other) const noexcept
{
	return std::tie(tm_year, tm_mon, tm_mday, tm_hour, tm_min, tm_sec) !=
		std::tie(other.tm_year, other.tm_mon, other.tm_mday, other.tm_hour, other.tm_min, other.tm_sec);
}

sakura::string sakura::DateTime::to_string() const noexcept
{
	return sakura::date_time_string(*this);
}

sakura::time_t sakura::now() noexcept
{
	return time(0);
}

sakura::DateTime sakura::to_date_time(const sakura::time_t* time) noexcept
{
	std::tm* res = std::localtime(time);
	return *reinterpret_cast<sakura::DateTime*>(res);
}

sakura::string sakura::date_time_string(const sakura::DateTime& dt) noexcept
{
	return sakura::to_string(dt.tm_year + 1900).append("-")
		.append(sakura::to_string(dt.tm_mon + 1)).append("-")
		.append(sakura::to_string(dt.tm_mday)).append(" ")
		.append(sakura::to_string(dt.tm_hour)).append(":")
		.append(sakura::to_string(dt.tm_min)).append(":")
		.append(sakura::to_string(dt.tm_sec)).c_str();
}


std::thread::id sakura::Core::main_thread_id_;
void sakura::Core::initialize(const Parameters& params) noexcept
{
	main_thread_id_ = std::this_thread::get_id();
#ifdef _DEBUG
	sakura::Logger::set_log_level(sakura::Logger::ELevel::debug);
#else
	sakura::Logger::set_log_level(sakura::Logger::ELevel::warn);
#endif
	// Setup project dir for dev_local.
	sakura::vfs::dev_local::adapter_dev_local::set_project_dir(params.project_dir);

	// Regist DevLocal Adapter
	auto adp = vfs::dev_local::adapter_dev_local::get();
	auto sym = adp->get_symbol();
	sakura::vfs::adapter::mounted_adapters.insert_or_assign(sym, adp);
}

std::thread::id sakura::Core::get_main_thread_id() noexcept
{
	return main_thread_id_;
}


void RuntimeCoreModule::StartUp()
{
    std::cout << "RuntimeCoreModule StartUp!" << std::endl;    
}
