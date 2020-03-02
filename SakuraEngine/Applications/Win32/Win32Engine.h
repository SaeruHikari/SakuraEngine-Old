/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 * 
 * 
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:59
 * @LastEditTime: 2020-03-02 15:00:28
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