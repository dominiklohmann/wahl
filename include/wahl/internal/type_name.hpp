// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <string_view>

namespace wahl::internal {

namespace type_name_probe {

/// A templated function whose whole purpose is to embed the name of an
/// explicitly specified template parameter into its signature, which the
/// function returns via platform-specific macros.
template <class T> constexpr std::string_view embed() {
#if defined(_MSC_VER)
  return __FUNCSIG__;
#else
  return __PRETTY_FUNCTION__;
#endif
}

/// A probe with a known type; used for extracting the platform-specific prefix
/// and suffix from the result of embed<T>.
constexpr auto probe = embed<double>();
constexpr auto prefix_length = probe.find("double");
constexpr auto suffix_length =
    probe.size() - prefix_length - std::string_view("double").size();

// Sanity check that the probe did contain the type name.
static_assert(prefix_length != std::string_view::npos,
              "cannot extract typename from function signature");

} // namespace type_name_probe

/// Retrieves the type name of **T** at compile time.
template <class T> constexpr std::string_view type_name() {
  auto name = type_name_probe::embed<T>();
  name.remove_prefix(type_name_probe::prefix_length);
  name.remove_suffix(type_name_probe::suffix_length);
  return name;
}

} // namespace wahl::internal
