/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-10 14:10:15
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 19:11:08
 */
#pragma once
#include "Framework/Application/SWindow.h"
#include <xcb/xcb.h>

namespace Sakura
{
    struct SLinuxWindowInfo
    {
        xcb_connection_t*       pConn = nullptr;
        xcb_screen_t*           pScreen = nullptr;
        xcb_window_t            window;
        xcb_gcontext_t          foreground;
        xcb_gcontext_t          background;
        xcb_generic_event_t*    pEvent = nullptr;
        uint32                  mask = 0;
        uint32                  values[2];
        uint8                   isQuit = 0;
    };
    
    class SLinuxWindow : SImplements SWindow
    {
        friend class SLinuxEngine;
    private:
        // SWindow Interface Implements
        virtual void OnDestroy(void) override;
        virtual int OnCreate(void) override;            
    public:
        virtual void* ForceGetScreen(void) override
        {
            return (void*)windInfo.pScreen;
        }
        const SLinuxWindowInfo& GetLinuxWindowInfo()
        {
            return windInfo;
        }
        SLinuxWindow() = default;
        SLinuxWindow(const WindowDesc& desc);
    private:
        SLinuxWindowInfo windInfo;
    };
}