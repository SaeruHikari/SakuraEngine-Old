/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-10 14:10:24
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 22:35:32
 */
#include "LinuxWindow.h"
#include "Core/EngineUtils/log.h"
#include <iostream>

namespace Sakura
{
    void SLinuxWindow::OnDestroy(void)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        Sakura::log::debug_info( "Window " + GetTitle() + " Destroyed.");
    #endif
        return;
    }

    int SLinuxWindow::OnCreate(void)
    {
    #ifdef SAKURA_DEBUG_EDITOR
         Sakura::log::debug_info( "Window " + GetTitle() + " Created.");
    #endif
        return 0;
    }

    SLinuxWindow::SLinuxWindow(const WindowDesc& desc)
        :Sakura::SWindow(desc)
    {
        windInfo.pConn = xcb_connect(0, 0);
        windInfo.pScreen = xcb_setup_roots_iterator(xcb_get_setup(windInfo.pConn)).data;
        windInfo.window = windInfo.pScreen->root;
        
        windInfo.foreground = xcb_generate_id(windInfo.pConn);
        windInfo.mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
        windInfo.values[0] = windInfo.pScreen->black_pixel;
        windInfo.values[1] = 0;
        xcb_create_gc(windInfo.pConn, windInfo.foreground,
            windInfo.window, windInfo.mask, windInfo.values);

        /* create white (background) graphic context */
        windInfo.background = xcb_generate_id(windInfo.pConn);
        windInfo.mask = XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES;
        windInfo.values[0] = windInfo.pScreen->white_pixel;
        windInfo.values[1] = 0;
        xcb_create_gc(windInfo.pConn, windInfo.background, 
            windInfo.window, windInfo.mask, windInfo.values);

        /* create window */
        windInfo.window = xcb_generate_id(windInfo.pConn);
        windInfo.mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        windInfo.values[0] = windInfo.pScreen->white_pixel;
        windInfo.values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
        xcb_create_window (windInfo.pConn,                                       /* connection */
                            XCB_COPY_FROM_PARENT,        /* depth */
                            windInfo.window,                                      /* window ID */
                            windInfo.pScreen->root,                       /* parent window */
                            desc.AncorX, desc.AncorY,                                      /* x, y */
                            desc.width, desc.height,                            /* width, height */
                            10,                                          /* boarder width */
                            XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class */
                            windInfo.pScreen->root_visual,        /* visual */
                            windInfo.mask, windInfo.values);                       /* masks */
        
        volatile size_t test = sizeof(char);

        /* set the title of the window */
        xcb_change_property(windInfo.pConn, XCB_PROP_MODE_REPLACE,
                            windInfo.window,
                            XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                            desc.title.size(), desc.title.c_str());
        // icon 
        char title_icon[] = "Hello, Engine! (iconified)";
        /* set the title of the window icon */
        xcb_change_property(windInfo.pConn, XCB_PROP_MODE_REPLACE, windInfo.window,
                            XCB_ATOM_WM_ICON_NAME, XCB_ATOM_STRING, 8,
                            strlen(title_icon), title_icon);

        /* map the window on the screen */
        xcb_map_window(windInfo.pConn, windInfo.window);

        xcb_flush(windInfo.pConn);

        while((windInfo.pEvent = xcb_wait_for_event(windInfo.pConn)) && !windInfo.isQuit) {
                switch(windInfo.pEvent->response_type & ~0x80) {
                case XCB_EXPOSE:
                        break;
                case XCB_KEY_PRESS:
                        windInfo.isQuit = 1;
                        break;
                }
                free(windInfo.pEvent);
        }
    }
}