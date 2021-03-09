// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/internal/is_container.hpp>
#include <wahl/value_parser.hpp>

namespace wahl::internal {

template <class T,
          std::enable_if_t<
              std::disjunction_v<std::negation<internal::is_container<T>>,
                                 std::is_convertible<T, std::string>>,
              int> = 0>
void absorb(T &result, const std::string &x) {
  result = value_parser<T>::apply(x);
}

template <
    class T,
    std::enable_if_t<
        std::conjunction_v<internal::is_container<T>,
                           std::negation<std::is_convertible<T, std::string>>>,
        int> = 0>
void absorb(T &result, const std::string &x) {
  result.insert(result.end(), value_parser<typename T::value_type>::apply(x));
}

inline void absorb(std::nullptr_t, const std::string &) {
  // nop
}

} // namespace wahl::internal
