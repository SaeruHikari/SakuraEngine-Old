#pragma once
#include "Framework/Application/SEngine.h"

namespace Sakura
{
	class SWin32Engine : SImplements SEngine
	{
		friend SInterface SEngine;
		SWin32Engine(const EngineDesc& _desc)
		{

		}
	public:
		virtual bool Initialize(void) override;
		virtual int Run(void) override;
		virtual bool Destroy(void) override;
	};
}