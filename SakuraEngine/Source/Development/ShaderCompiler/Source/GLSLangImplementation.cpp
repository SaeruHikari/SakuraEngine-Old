#ifdef SAKURA_TARGET_PLATFORM_WIN
#include "ShaderCompiler/ShaderCompiler.h"

ShaderCompilerAPI sakura::vector<std::byte> sakura::development::compile_glsl(
	const sakura::string& code, const ShaderCompilerVariables& vars)
{
	using namespace sakura::graphics;
	assert(vars.target_language == EShaderLanguage::Spirv && "GLSLang Backend Only Supports Spirv!");


	std::string out;

	sakura::vector<std::byte> buffer(out.length());
	memcpy(buffer.data(), out.data(), out.length());
	return buffer;
}

#endif