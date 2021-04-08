#include <iosfwd>
#include <string>
#include <string_view>
#include <type_traits>
#include <cstdint>
#include "config.hpp"

#pragma once

namespace rustpp {

	using u8 = const std::uint8_t;
	using u16 = const std::uint16_t;
	using u32 = const std::uint32_t;
	using u64 = const std::uint64_t;
	using i8 = const std::int8_t;
	using i16 = const std::int16_t;
	using i32 = const std::int32_t;
	using i64 = const std::int64_t;
	using f32 = const float;
	using f64 = const double;
	using usize = const std::size_t;
	using isize = const std::ptrdiff_t;
	using rool = const bool;
	using String = const std::string;
	using str = std::string_view;

	template<typename T>
	struct mut;

	template<typename T> requires (std::is_reference_v<T> == false)
		struct mut<T>
	{
		using type = std::decay_t<T>;
		type t;
		mut(const type &a) noexcept : t{ a } {}
		mut &operator=(const type &a) noexcept { t = a; return *this; }
		operator type &() noexcept { return t; }
	};

	template<typename T> requires std::is_reference_v<T>
		struct mut<T>
		{
			using type = std::decay_t<T>;
			type &t;
			explicit mut(mut<const type> &a) noexcept : t{ a.t } {}
			mut &operator=(const type &a) noexcept { t = a; return *this; }
			operator type &() noexcept { return t; }
		};

		template<typename T>
		std::ostream &operator<<(std::ostream &out, const mut<T> &m) {
			return out << m.t;
		}

		template<typename T> mut(mut<T> &)->mut<T &>;

} // namespace rustpp.