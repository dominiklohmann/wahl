// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/internal/is_container.hpp>

namespace wahl {

enum class argument_type { none, single, multiple };

namespace internal {

template <class T> argument_type infer_argument_type(const T&) {
  if constexpr (std::is_same_v<T, bool> or std::is_same_v<T, std::nullptr_t>)
    return argument_type::none;
  else if constexpr (is_container_v<T>)
    return argument_type::multiple;
  else
    return argument_type::single;
}

} // namespace internal

} // namespace wahl
