// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <deque>
#include <functional>
#include <map>
#include <string>

namespace wahl {

template <class... Args> struct subcommand {
  std::string help;
  std::function<void(std::deque<std::string>, Args...)> run;

  using map = std::map<std::string, subcommand>;
};

} // namespace wahl
