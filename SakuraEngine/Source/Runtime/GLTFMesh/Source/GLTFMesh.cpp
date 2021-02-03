#include "GLTFMesh/GLTFMesh.h"
#define SIMDJSON_USING_LIBRARY 1
#include "Base/SIMDJson.h"
#include <iostream>

GLTFMeshAPI void gltf_load(const char* gltf, GLTFFile* result)
{
    using namespace simdjson;
    using namespace std;

    auto abstract_json = R"( [
    {  "12345" : {"a":12.34, "b":56.78, "c": 9998877}   },
    {  "12545" : {"a":11.44, "b":12.78, "c": 11111111}  }
    ] )"_padded;
    auto padded_gltf = simdjson::padded_string(gltf, ::strlen(gltf));
    dom::parser parser;
    for (dom::object gltfObj : parser.parse(padded_gltf)) 
    {
        cout << std::string_view(gltfObj["asset"]["extras"]["author"]);
    }

    // Parse and iterate through an array of objects
    for (dom::object obj : parser.parse(abstract_json)) {
        for(const auto key_value : obj) {
        cout << "key: " << key_value.key << " : ";
        dom::object innerobj = key_value.value;
        cout << "a: " << double(innerobj["a"]) << ", ";
        cout << "b: " << double(innerobj["b"]) << ", ";
        cout << "c: " << int64_t(innerobj["c"]) << endl;
        }
    }
}