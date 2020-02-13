/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-12 16:25:49
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 17:18:13
 */
#pragma once
#include "Framework/Application/SEngine.h"
#include "Core/CoreMinimal/CoreMinimal.h"

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
		void __init_win32(HINSTANCE _hInstance, HINSTANCE _hPrevInstance,
			LPTSTR _lpCmdLine, int _nCmdShow)
		{
			hInstance = _hInstance;
			hPrevInstance = _hPrevInstance;
			lpCmdLine = _lpCmdLine;
			nCmdShow = _nCmdShow;
		}
	private:
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		LPTSTR lpCmdLine;
		int nCmdShow;
	};
}