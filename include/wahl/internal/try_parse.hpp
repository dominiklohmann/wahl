// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/internal/rank.hpp>

#include <utility>

namespace wahl::internal {

/// Invokes `x.parse(f)` if possible, or does nothing.
template <class T, class F> auto try_parse(T &x, F &&f) {
  return try_parse(internal::rank_v<1>, x, std::forward<F>(f));
}

template <class T, class F>
auto try_parse(internal::rank<1>, T &x, F &&f) -> decltype(x.parse(std::forward<F>(f))) {
  return x.parse(std::forward<F>(f));
}

template <class T, class F> void try_parse(internal::rank<0>, T &, F &&) {
  // nop
}

} // namespace wahl::internal
