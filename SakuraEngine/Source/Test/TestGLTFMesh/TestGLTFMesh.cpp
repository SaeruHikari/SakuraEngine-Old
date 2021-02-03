#include "GLTFMesh/GLTFMesh.h"
#include "System/IO.h"
#include <iostream>

int main(void)
{
	int64_t size = ::fsize("./scene.gltf");

	::FILE* gltfFile = ::fopen("scene.gltf", "r");
	if(!gltfFile)
	{
		return 1;
	}

	std::cout << size;

	const char* gltfSrc = nullptr;
	//gltf_load(gltfSrc, nullptr);
	return 0;
}
