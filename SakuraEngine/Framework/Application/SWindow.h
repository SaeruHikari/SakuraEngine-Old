/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Window Interface of SakuraEngine
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:43:36
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 17:14:10
 */
#pragma once
#include "Core/CoreMinimal/CoreMinimal.h"
#include "Core/Containers/SString.h"
#include "Core/Math/Color.h"
#include <memory>

namespace Sakura
{
    struct WindowDesc
    {
        float width = 800;
        float height = 600;
        float AncorX = 300;
        float AncorY = 300;
        bool bRegisterEx = true;
        Math::Unorm4 forground;
        Math::Unorm4 background;
        Sakura::spmr_string title = "SakuraEngine Window";
    };

    sinterface SWindow
    {
        // Interfaces
        SWindow() = default;
        SWindow(const WindowDesc& _desc)
            :desc(_desc)
        {} 
        /**
         * @description: Force Get Window 
         * @param void 
         * @return: void* wind ptr
         * @author: SaeruHikari
         */
        virtual void* ForceGetScreen(void) = 0;
        virtual void OnResize(float width, float height)
        {
            desc.height = height;
            desc.width = width;
        }
        virtual void OnDestroy(void) = 0;
        virtual int OnCreate(void) = 0;
        // properties get;set;

        virtual void SetTile(const Sakura::spmr_string& str) { desc.title = str;}
      
        sinline Sakura::spmr_string GetTitle(void) {return desc.title;}
        sinline float GetHeight(void){return desc.height;}
        sinline float GetWidth(void) {return desc.width;}
        sinline Math::Unorm4 GetBackgroundColor(void){return desc.background;}
        sinline Math::Unorm4 GetForeGroundColor(void){return desc.forground;}
    protected:
        WindowDesc desc;
    };
}
