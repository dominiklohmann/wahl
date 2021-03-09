// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <experimental/type_traits>

#include <string>
#include <string_view>
#include <type_traits>

namespace wahl::internal {

/// Type trait for determining container types, i.e., types that work with
/// `std::data` and `std::size`. Explicitly disabled for specializations of
/// `std::basic_string` and `std::basic_string_view`.
template <class T, class = void> struct is_container : std::false_type {};

template <class T> using std_data_t = decltype(std::data(std::declval<T>()));
template <class T> using std_size_t = decltype(std::size(std::declval<T>()));

template <class T> struct is_container_disabled : std::false_type {};

template <class CharT, class Traits, class Allocator>
struct is_container_disabled<std::basic_string<CharT, Traits, Allocator>>
    : std::true_type {};

template <class CharT, class Traits>
struct is_container_disabled<std::basic_string_view<CharT, Traits>>
    : std::true_type {};

template <class T>
struct is_container<T, std::enable_if_t<std::conjunction_v<
                           std::experimental::is_detected<std_data_t, T>,
                           std::experimental::is_detected<std_size_t, T>,
                           std::negation<is_container_disabled<T>>>>>
    : std::true_type {};

template <class T>
inline constexpr bool is_container_v = is_container<T>::value;

} // namespace wahl::internal
