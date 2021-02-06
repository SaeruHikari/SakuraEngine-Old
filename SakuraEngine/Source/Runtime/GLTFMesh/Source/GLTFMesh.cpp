#include "GLTFMesh/GLTFMesh.h"
#include "System/Memory.h"
#include "SIMDJson/SIMDJson.h"
#include <iostream>

GLTFMeshAPI GLTFFile gltf_load(const char* gltf)
{
    using namespace simdjson;
    using namespace std;

    auto gltfl = ::strlen(gltf);
    auto padded_gltf = simdjson::padded_string(gltf, gltfl);
    dom::parser parser;
    dom::object gltfObj = parser.parse(padded_gltf);
    
    // accessors   
    dom::array accessors_ = dom::array(gltfObj["accessors"]);
    for(size_t i = 0; i < accessors_.size(); i++)
    {
        auto bView = accessors_.at(i)["bufferView"];
        std::cout << bView;
    }

    GLTFFile result = {
        
    };
    return result;
}