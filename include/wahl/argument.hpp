// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/argument_type.hpp>

#include <numeric>
#include <vector>
#include <string>
#include <functional>

namespace wahl {

template <class Range> std::string join(Range &&r, std::string delim) {
  return std::accumulate(std::begin(r), std::end(r), std::string(),
                         [&](const std::string &x, const std::string &y) {
                           if (x.empty())
                             return y;
                           if (y.empty())
                             return x;
                           return x + delim + y;
                         });
}

struct argument {
  argument_type type;
  std::vector<std::string> flags;

  int count = 0;
  bool required = false;
  std::function<void(const std::string &)> write_value;
  std::vector<std::function<void(const argument &)>> callbacks;
  std::vector<std::function<void(const argument &)>> eager_callbacks;
  std::string help, metavar;

  template <class F> void add_callback(F f) {
    callbacks.emplace_back(std::move(f));
  }

  template <class F> void add_eager_callback(F f) {
    eager_callbacks.emplace_back(std::move(f));
  }

  std::string get_flags() const {
    std::string result = join(flags, ", ");
    if (type != argument_type::none)
      result += " " + metavar;
    return result;
  }

  bool write(const std::string &s) {
    this->write_value(s);
    count++;
    for (auto &&f : eager_callbacks)
      f(*this);
    return not eager_callbacks.empty();
  }
};

} // namespace wahl
