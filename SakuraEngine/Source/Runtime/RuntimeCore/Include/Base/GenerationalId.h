#pragma once
#include "Base/Definitions.h"
#include "spdlog/fmt/fmt.h"

namespace sakura
{
	namespace ____
	{
		template<bool True> struct uhalf {};
		template<> struct uhalf<sizeof(size_t) == sizeof(uint32)> { using type = uint16; constexpr static uint16 max = UINT16_MAX; };
		template<> struct uhalf<sizeof(size_t) == sizeof(uint64)> { using type = uint32; constexpr static uint32 max = UINT32_MAX; };
		static_assert(sizeof(uhalf<true>::type) == sizeof(size_t) / 2, "uhalf type error! size should be half as size_t.");
	};
}

namespace sakura
{
	struct RuntimeCoreAPI GenerationalId
	{
		using uhalf = ____::uhalf<true>;
		using uhalf_t = uhalf::type;
		using generation_t = uhalf_t;
		using index_t = uhalf_t;

		static const GenerationalId UNINITIALIZED;

		GenerationalId(uhalf_t generation, uhalf_t index);
		GenerationalId();
		operator size_t() const;
		explicit operator bool() const;
		FORCEINLINE bool operator==(const GenerationalId &other) const
		{
			return this->storage() == other.storage();
		}
		size_t storage() const;
		generation_t generation() const;
		index_t index() const;
	protected:
		union
		{
			size_t storage;
			struct view
			{
				uhalf_t generation;
				uhalf_t index;
			}v;
		}u;

		explicit GenerationalId(uint64 storage);
	};
}

namespace std
{
	template<> struct hash<sakura::GenerationalId>
	{
		sakura::size_t operator()(sakura::GenerationalId id) const
		{
			return std::hash<sakura::uint64>()(id.storage());
		}
	};
}

namespace fmt
{
	template<> struct formatter<sakura::GenerationalId> : fmt::formatter<std::string_view>
	{
		template<typename FormatContext>
		auto format(sakura::GenerationalId id, FormatContext& ctx)
		{
			return fmt::format_to(
				ctx.out(),
				"gen: {} <-> id: {}", id.generation(), id.index()
			);
		}
	};
}