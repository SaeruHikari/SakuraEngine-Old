#pragma once

namespace Sakura::concepts
{
	template<typename T>
	struct remove_rvalue_reference
	{
		using type = T;
	};

	template<typename T>
	struct remove_rvalue_reference<T&&>
	{
		using type = T;
	};

	template<typename T>
	using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;
}