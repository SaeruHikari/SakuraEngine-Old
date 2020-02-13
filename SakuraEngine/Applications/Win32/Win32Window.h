/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-12 16:28:16
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 17:23:20
 */
#pragma once
#include "Framework/Application/SWindow.h"
#include "Core/CoreMinimal/CoreMinimal.h"

namespace Sakura
{
    struct SWin32WindowInfo
    {
        HWND hwnd;
    };

    class SWin32Window : SImplements SWindow
    {
        friend class SWin32Engine;
        friend class SEngine;
    private:
        virtual void OnDestroy(void) override;
        virtual int OnCreate(void) override;
    public:
        virtual void* ForceGetScreen(void) override
        {
            return nullptr;
            //return windInfo.hwnd;
        }
        const SWin32WindowInfo& GetWin32WindowInfo()
        {
            return windInfo;
        }
        SWin32Window() = default;
        SWin32Window(const WindowDesc& desc);
    private:
        int ____create(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPTSTR lpCmdLine, int nCmdShow);
        SWin32WindowInfo windInfo;
    };
}