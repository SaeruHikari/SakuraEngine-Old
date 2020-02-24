/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-10 19:08:16
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-24 11:38:40
 */
#pragma once
#include "SApplication.h"

namespace Sakura
{
    SInterface SEngine;
}

namespace Sakura
{
    namespace ____
    {
        extern SEngine* eEngine;
    }
    struct EngineDesc
    {
        
    };
    SInterface SEngine : SImplements SApplication
    {
        SEngine(const EngineDesc& _desc)
            :desc(_desc){}
        SEngine() = default;
        SAKURA_API static void CreateSEngine(const EngineDesc& _desc);
        SAKURA_API static std::unique_ptr<SWindow> CreateSWindow(const WindowDesc& desc);
    public:
        // get;
        SAKURA_API static EngineDesc GetEngineDesc(void);
        SAKURA_API static SEngine* GetEngine(void);
    private:
        EngineDesc desc;
    };

}