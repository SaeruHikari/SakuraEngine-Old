#include "ECS/ECS.h"

ECSModule* ECSModule::create()
{
	return new ECSModule();
}

bool ECSModule::StartUp()
{
	core::database::initialize();
	core::codebase::initialize();

	core::codebase::cid<core::database::group> = core::database::group_id;
	core::codebase::cid<core::database::disable> = core::database::disable_id;
	core::codebase::cid<core::database::cleanup> = core::database::cleanup_id;
	core::codebase::cid<core::database::mask> = core::database::mask_id;

	return true;
}

bool ECSModule::Terminate()
{
	return true;
}
