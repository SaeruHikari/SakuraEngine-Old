#include "RuntimeCore/Resource.h"
#include "System/Log.h"

using namespace sakura;

//sakura::unordered_map<resource::ResourceId, sakura::vector<sakura::vfs::path>> resource::Resource::resources_path_map_;
//sakura::unordered_map<string, resource::ResourceId> resource::Resource::resources_name_map_;
vector<pair<resource::Resource*, resource::ResourceHandle::generation_t>> resource::ResourceHandle::global_resources_ = {};
bool resource::ResourceHandle::valid() const
{
	if(global_resources_.size() < index() + 1)
	{
		sakura::error("[Resource]: Handle has an invalid index oversized as {}", *this);
		return false;
	}
	auto& infoPair = global_resources_[index()];
	if(infoPair.second != generation())
	{
		sakura::error("[Resource]: Handle Access to an invalid generation!");
		return false;
	}
	return global_resources_[index()].first->valid();
}

resource::Resource& resource::ResourceHandle::get() const
{
	if (!valid())
	{
		sakura::error("[Resource]: Not Loaded Up!");
		assert(0 && "Check Console Log Error Message.");
	}
	return *global_resources_[index()].first;
	
}

