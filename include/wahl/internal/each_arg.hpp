// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <utility>

namespace wahl::internal {

/// Calls `f` for every other argument.
template <class F, class T, class... Ts> void each_arg(F f, T &&x, Ts &&...xs) {
  static_cast<void>(f(std::forward<T>(x)));
  each_arg(f, std::forward<Ts>(xs)...);
}

template <class F> void each_arg(F) {
  // nop
}

} // namespace wahl::internal
