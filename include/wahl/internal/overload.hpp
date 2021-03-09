// SPDX-License-Identifier: BSL-1.0

#pragma once

namespace wahl::internal {

/// Creates a set of overloaded functions. This utility struct allows for
/// writing inline visitors without having to result to inversion of control.
template <class... Ts> struct overload : Ts... { using Ts::operator()...; };

/// Explicit deduction guide for overload.
template <class... Ts> overload(Ts...) -> overload<Ts...>;

} // namespace wahl::internal
