// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <string>
#include <sstream>

namespace wahl {

template <class T> struct value_parser {
  static T apply(const std::string &x) {
    T result;
    std::stringstream ss;
    ss.str(x);
    ss >> result;
    return result;
  }
};

} // namespace wahl
