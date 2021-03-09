// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/internal/rank.hpp>

#include <string>

namespace wahl::internal {

/// Invokes `Command::options_metavar()` if possible and convertible to
/// `std::string`, or returns the default string.
template <class Command> auto try_options_metavar() {
  return try_options_metavar<Command>(rank_v<1>);
};

template <class Command>
auto try_options_metavar(rank<1>)
    -> decltype(std::string{Command::options_metavar()}) {
  return std::string{Command::options_metavar()};
}

template <class Command> auto try_options_metavar(rank<0>) -> std::string {
  return "[options...]";
};

} // namespace wahl::internal
