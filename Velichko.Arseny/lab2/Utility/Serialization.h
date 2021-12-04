#pragma once

#include <type_traits>

namespace Serialization {
	template<class T>
	using CopyableCheck = std::enable_if_t<std::is_trivially_copyable<T>::value>;

	template<class T, class = CopyableCheck<T>>
	void serialize(char* buf, const T& value);

	template<class T, class = CopyableCheck<T>>
	T parse(const char* buf);
}

#include "Serialization.tpp"