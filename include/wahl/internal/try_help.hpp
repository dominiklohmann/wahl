// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/internal/rank.hpp>

#include <string>

namespace wahl::internal {

/// Invokes `Command::help()` if possible and convertible to `std::string`, or
/// returns an empty string.
template <class Command> auto try_help() {
  return try_help<Command>(rank_v<1>);
};

template <class Command>
auto try_help(rank<1>) -> decltype(std::string{Command::help()}) {
  return std::string{Command::help()};
}

template <class Command> auto try_help(rank<0>) -> std::string { return {}; };

} // namespace wahl::internal
