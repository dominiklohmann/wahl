// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/internal/is_container.hpp>
#include <wahl/internal/rank.hpp>

namespace wahl::internal {

template <class T> auto try_type_to_help() {
  return try_type_to_help<T>(rank_v<1>);
}

template <class T>
auto try_type_to_help(rank<1>) -> std::enable_if_t<
    std::conjunction_v<is_container_v<T>,
                       std::negation<std::is_convertible<T, std::string>>>,
    std::string> {
  return try_type_to_help<typename T::value_type>(rank_v<1>) + "...";
}

template <class T> auto try_type_to_help(internal::rank<0>) -> std::string {
  if constexpr (std::is_same_v<T, bool>)
    return "bool";
  else if constexpr (std::is_convertible_v<T, std::string>)
    return "string";
  else if constexpr (std::is_integral_v<T>)
    return "integer";
  else if constexpr (std::is_floating_point_v<T>)
    return "number";
  else
    return "argument";
}

template <class T> std::string type_to_help(const T &) {
  return "[" + try_type_to_help<T>() + "]";
}

} // namespace wahl::internal
