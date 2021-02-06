#pragma once
#include "Base/CDefinitions.h"

extern "C"
{
	typedef struct GLTFAccessor
	{
		const uint64_t buffer_view;
		const uint64_t byte_offset;
		const uint32_t component_type;
		const uint32_t count;
		const float* max;
		const uint32_t max_dimension;
		const float* min;
		const uint32_t min_dimension;
		const char* type;
	} GLTFAccessor;
	typedef struct GLTFBufferView
	{
		const uint32_t buffer;
		const uint64_t byte_length;
		const uint64_t byte_offset;
		const char* name;
		uint32_t target;
	} GLTFBufferView;
	typedef struct GLTFAttribute
	{
		const char* name;
		const uint32_t accessor;
	} GLTFAttribute;
	typedef struct GLTFPrimitive
	{
		const GLTFAttribute* attributes;
		const uint32_t attributes_count;
		const uint64_t indices;
	} GLTFPrimitive;
	typedef struct GLTFMesh
	{
		const GLTFPrimitive* primitives;
		const uint32_t primitives_count;
	} GLTFMesh;
	typedef struct GLTFBuffer
	{
		const uint64_t byte_length;
		const char* uri;
	} GLTFBuffer;
	typedef struct GLTFNode
	{
		const uint32_t* children;
		const uint32_t children_count;
	} GLTFNode;
	typedef struct GLTFScene
	{
		const char* name;
		const uint32_t* nodes;
		const uint32_t nodes_count;
	} GLTFScene;
	typedef struct GLTFFile
	{
		const GLTFAccessor* accessors;
		const uint32_t accessors_count;

		const GLTFBufferView* buffer_views;
		const uint32_t buffer_views_count;

		const GLTFBuffer* buffers;
		const uint32_t buffers_count;

		const GLTFScene* scenes;
		const uint32_t scenes_count;
		
		const uint32_t scene;
	} GLTFFile;
	typedef buffer GLTFBin;
	typedef buffer_view GLTFBinView;

	GLTFMeshAPI GLTFFile gltf_load(const char* gltf);
	GLTFMeshAPI void gltf_load_bin(const GLTFFile* file,
		GLTFBin* bins, uint32_t bins_count);
	GLTFMeshAPI void gltf_restore_bin(const GLTFFile* file,
		GLTFBinView* bin_views, uint32_t bin_views_count);
	GLTFMeshAPI void gltf_free(GLTFFile* gltf);
}


#ifdef __cplusplus
namespace sakura::gltf
{
	struct Accessor : ::GLTFAccessor
	{

	};

	struct BufferView : ::GLTFBufferView
	{

	};

	struct Attribute : ::GLTFAttribute
	{

	};

	struct Primitive : ::GLTFPrimitive
	{

	};

	struct Mesh : ::GLTFMesh
	{

	};
	
	struct Buffer : ::GLTFBuffer
	{
		
	};
	
	struct Node : ::GLTFNode
	{

	};

	struct Scene : ::GLTFScene
	{

	};

	struct Bin : ::GLTFBin
	{

	};

	struct BinView : ::GLTFBinView
	{

	};

	struct GLTFMeshAPI File : ::GLTFFile
	{
		void load_bin(Bin* bins, uint32_t bins_count);
		void restore_bin(BinView* bin_views, uint32_t bin_views_count);
	};

	[[nodiscard]] GLTFMeshAPI File* load(const char* gltf);
}
#endif

extern "C"
{
	static const uint32_t GLTF_TARGET_ARRAY_BUFFER = 34962;
	static const uint32_t GLTF_TARGET_ELEMENT_ARRAY_BUFFER = 34963;

	static const uint32_t GLTF_COMP_TYPE_BYTE = 5120;//1
	static const uint32_t GLTF_COMP_TYPE_UBYTE = 5121;//1
	static const uint32_t GLTF_COMP_TYPE_SHORT = 5122;//2
	static const uint32_t GLTF_COMP_TYPE_USHORT = 5123;//2
	static const uint32_t GLTF_COMP_TYPE_UINT = 5125;//4
	static const uint32_t GLTF_COMP_TYPE_FLOAT = 5126;//4

	static const char* GLTF_TYPENAME_SCALAR = "SCALAR";
	static const char* GLTF_TYPENAME_VEC2 = "VEC2";
	static const char* GLTF_TYPENAME_VEC3 = "VEC3";
	static const char* GLTF_TYPENAME_VEC4 = "VEC4";
	static const char* GLTF_TYPENAME_MAT2 = "MAT2";
	static const char* GLTF_TYPENAME_MAT3 = "MAT3";
	static const char* GLTF_TYPENAME_MAT4 = "MAT4";
}