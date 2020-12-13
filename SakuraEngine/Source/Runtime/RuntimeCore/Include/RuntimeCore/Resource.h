#pragma once
#include "base/GUID.h"
#include "base/GenerationalId.h"
#include "system/VirtualFileSystem.h"

namespace sakura
{
    class Core;
}
namespace sakura::vfs
{
	struct path;
}

namespace sakura::resource
{
    using ResourceId = sakura::Guid;

    // Resource: Convert From Assets.
	//  Resource::load<ResT>(ResourceId)
	//  ResourceId = Resource::create
    struct RuntimeCoreAPI Resource
    {
        virtual bool valid() const = 0;

    protected:
        //static sakura::unordered_map<ResourceId, sakura::vector<sakura::vfs::path>> resources_path_map_;
        //static sakura::unordered_map<string, ResourceId> resources_name_map_;
        ResourceId id_;
        vector<ResourceId> references_;
    };
    
    struct RuntimeCoreAPI ResourceHandle : public GenerationalId
    {
    	// whether the resource is completely loaded.
        bool valid() const;

        // get resource.
        Resource& get() const;
    protected:
        static sakura::vector<sakura::pair<Resource*, generation_t>> global_resources_;
    };
	
    template<typename ResourceType>
    struct TypedResourceHandle : public ResourceHandle
    {
        static_assert(std::is_base_of_v<Resource, ResourceType>, "Can't Instance a ResourceHandleTemplate with a non-resource type.");

        ResourceType* operator->();
    };

    template <typename ResourceType>
    ResourceType* TypedResourceHandle<ResourceType>::operator->()
    {
        return static_cast<ResourceType*>(&get());
    }

    // Type-Unsafe operations.
    namespace detail
    {
    	// Accesses VFS and load resource up.
        [[nodiscard]] RuntimeCoreAPI ResourceHandle load(const sakura::vfs::path& path);
    }
}
