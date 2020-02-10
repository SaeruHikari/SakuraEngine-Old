/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-10 19:08:16
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-10 20:26:26
 */
#pragma once
#include "SApplication.h"

namespace Sakura
{
    Interface SEngine;
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
    Interface SEngine : Implements SApplication
    {
        SEngine(const EngineDesc& _desc)
            :desc(_desc){}
        SEngine() = default;
        static void CreateEngine(const EngineDesc& _desc);
        static std::unique_ptr<SWindow> CreateWindow(const WindowDesc& desc);
    public:
        // get;
        sinline const static EngineDesc GetEngineDesc(void)
        {
            if(____::eEngine == nullptr)
                assert(0 && "Error! Engine not Created!");
            else
            {
                return ____::eEngine->desc;                
            }
            return EngineDesc();
        }
        sinline const static SEngine* GetEngine(void) 
        {
            if(____::eEngine == nullptr)
                assert(0 && "Error! Engine not Created!");
            else
            {
                return ____::eEngine;
            }
            return nullptr;
        }
    private:
        EngineDesc desc;
    };

}