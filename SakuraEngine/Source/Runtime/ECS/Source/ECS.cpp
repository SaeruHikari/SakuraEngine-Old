#include "ECS/ECS.h"

ECSModule* ECSModule::create()
{
	return new ECSModule();
}

bool ECSModule::StartUp()
{
	core::codebase::initialize();

	return true;
}

bool ECSModule::Terminate()
{
	return true;
}
