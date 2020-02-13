/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 17:31:49
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 17:52:55
 */
//#include "Core/Core.h"
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "Framework/Application/SEngine.h"

int main(void)
{
    Sakura::EngineDesc desc;
    Sakura::SEngine::CreateSEngine(desc);
    Sakura::WindowDesc windDesc;
    Sakura::SEngine::CreateSWindow(windDesc);
}
