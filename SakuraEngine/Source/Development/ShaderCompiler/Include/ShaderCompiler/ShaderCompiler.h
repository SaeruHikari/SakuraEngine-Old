#pragma once
#include <Containers/vector.hpp>
#include "RenderGraph/RenderGraphCore.h"

namespace sakura::development
{
    enum class EShaderLanguage
    {
        GLSL,
        Spirv,
        DXBC,
        DXIL
    };

    struct ShaderCompilerVariables
    {
        std::string entry;
        std::string name;
        graphics::EShaderFrequency freq;
        EShaderLanguage target_language;
    };

	ShaderCompilerAPI sakura::vector<std::byte> compile_hlsl(
        const sakura::string& code, const ShaderCompilerVariables& vars);

    ShaderCompilerAPI sakura::vector<std::byte> compile_glsl(
        const sakura::string& code, const ShaderCompilerVariables& vars);
}