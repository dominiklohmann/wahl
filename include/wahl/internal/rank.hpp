// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <cstddef>

namespace wahl::internal {

/// Enables manually specifying function overload resolution order.
/// A function that takes `rank<N>` as a parameter may be called with any
/// `rank<M>{}` where `M >= N`. The overload where `M - N` is closest to zero
/// will be chosen.
template <std::size_t N> struct rank : rank<N - 1> {};
template <> struct rank<0> {};

} // namespace wahl::internal
