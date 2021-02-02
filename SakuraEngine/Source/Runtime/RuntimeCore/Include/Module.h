#pragma once
#include <SakuraSTL.hpp>

namespace sakura
{
	struct RuntimeCoreAPI IModule
	{
	public:
		using constructor_t = sakura::function<IModule*()>;
		virtual ~IModule() = default;
		IModule() = default;
		
		virtual bool StartUp() = 0;
		virtual bool Terminate() = 0;
		static bool StartUp(const sakura::string& name);
		static bool Terminate(const sakura::string& name);
		struct RuntimeCoreAPI Registry
		{
			static bool regist(const sakura::string& moduleName, const constructor_t& constructor);
		};
	protected:
		static sakura::unordered_map<sakura::string, constructor_t> constructors_;
		static sakura::unordered_map<sakura::string, IModule*> modules_;
	};
}


