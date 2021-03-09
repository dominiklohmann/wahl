// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/internal/rank.hpp>
#include <wahl/internal/type_name.hpp>

#include <string>
#include <utility>

namespace wahl::internal {

template <class Predicate> std::string trim(const std::string &s, Predicate p) {
  auto wsfront = std::find_if_not(s.begin(), s.end(), p);
  auto wsback = std::find_if_not(s.rbegin(), s.rend(), p).base();
  return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

/// Invokes `Command::name()` if possible and convertible to `std::string`, or
/// stringifies the `Command` type name.
template <class Command> auto try_name() {
  return try_name<Command>(rank_v<1>);
}

template <class Command>
auto try_name(rank<1>) -> decltype(std::string{Command::name()}) {
  return std::string{Command::name()};
}

template <class Command> auto try_name(rank<0>) {
  auto name = std::string{type_name<Command>()};
  auto i = name.find("::");
  if (i != std::string::npos)
    name = name.substr(i + 2);
  if (name.back() == '&')
    name.pop_back();
  return trim(name, [](char c) { return c == '_'; });
};

} // namespace wahl::internal
