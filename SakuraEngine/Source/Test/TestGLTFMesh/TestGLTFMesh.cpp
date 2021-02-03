#include "GLTFMesh/GLTFMesh.h"
#include "System/IO.h"
#include "Containers/string.hpp"
#include <iostream>
#include "Containers/vector.hpp"

int main(void)
{
	system("chcp 65001");

	int64_t size = ::fsize("./scene.gltf");

	::FILE* gltfFile = ::fopen("scene.gltf", "r");
	if(!gltfFile) return 1;

	sakura::string gltfSrc; gltfSrc.resize(size);
	::fread(gltfSrc.data(), sizeof(char), size / sizeof(char), gltfFile);
	
	::GLTFFile gltf = gltf_load(gltfSrc.c_str());
	
	return 0;
}
