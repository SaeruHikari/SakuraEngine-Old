#pragma once
#include "Base/Definitions.h"

namespace sakura::type_system
{
	struct RuntimeCoreAPI Member
	{
		const char* name = nullptr;
		const char* type = "error_t";
	};
	struct RuntimeCoreAPI StaticMember : public Member
	{
		const char* value = nullptr;
	};
	struct RuntimeCoreAPI Field : public Member
	{
		uint64_t offset_of = 0;
		const char* default_value = nullptr;
	};
	struct RuntimeCoreAPI StaticField : public StaticMember
	{

	};
	struct RuntimeCoreAPI Attribute : public StaticMember
	{

	};
	static const Field cstruct_fields[] = {
		{"name", "c_str", 0},
		{"fields", "array: Field", sizeof(const char*)},
	};
	struct RuntimeCoreAPI CStruct
	{
		const char* name = "CStruct";
		const Field* fields = cstruct_fields;
		const StaticField* static_fields = nullptr;
		const Attribute* attributes = nullptr;
	};
	struct RuntimeCoreAPI TypeAssembly
	{
		const char* name = "Default";
		const char* types = "";
	};
}