/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Window Interface of SakuraEngine
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:43:36
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 01:40:27
 */
#pragma once
#include "Core/CoreMinimal/CoreMinimal.h"
#include "Core/Containers/SString.h"
#include <memory>

namespace Sakura
{
    struct WindowDesc
    {
        float width;
        float height;
        Sakura::spmr_wstring title = L"SakuraEngine Window";
    };

    Interface SWindow
    {
        // Operate on window entity with EXTERN VAR
        // with different types on different platforms.
        // Like this:
        // extern hwnd win32Window;
        // ....

        // Interfaces
        
        /**
         * @description: Force Get Window 
         * @param void 
         * @return: void* wind ptr
         * @author: SaeruHikari
         */
        virtual void* ForceGetWindow(void) = 0;
        
        virtual void OnResize(float width, float height) = 0;

        virtual void OnDestroy(void) = 0;

        virtual int OnCreate(void) = 0;

        // properties get;set;

        virtual void SetTile(const Sakura::spmr_wstring& str) { desc.title = str;}
        virtual void SetHeight(float height) { desc.height = height;}
        virtual void SetWidth(float width) { desc.width = width;}

        sinline Sakura::spmr_wstring GetTitle(void) {return desc.title;}
        sinline float GetHeight(void){return desc.height;}
        sinline float GetWidth(void) {return desc.width;}
    private:
        WindowDesc desc;
    };
}
