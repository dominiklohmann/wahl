// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/argument.hpp>
#include <wahl/internal/absorb.hpp>
#include <wahl/internal/each_arg.hpp>
#include <wahl/internal/overload.hpp>
#include <wahl/internal/try_name.hpp>
#include <wahl/internal/type_to_help.hpp>
#include <wahl/subcommand.hpp>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace wahl {

std::vector<std::string> wrap(const std::string &text,
                              unsigned int line_length = 72);

template <class... Args> struct context {
  using subcommand_type = subcommand<Args...>;
  using subcommand_map = typename subcommand_type::map;

  std::vector<argument> arguments;
  std::unordered_map<std::string, int> lookup;
  subcommand_map subcommands;

  bool has_subcommand(const std::string &argv) {
    const auto current_name =
        internal::try_name<std::tuple_element_t<0, std::tuple<Args...>>>();
    return subcommands.find(argv) != subcommands.end() and argv != current_name;
  }

  bool has_default_capture() { return lookup.find("") != lookup.end(); }

  void add(argument arg) {
    if (arg.flags.empty())
      lookup[""] = arguments.size();
    else
      for (auto &&name : arg.flags)
        lookup[name] = arguments.size();
    arguments.emplace_back(std::move(arg));
  }

  template <class T, class... Ts> void parse(T &&x, Ts &&...xs) {
    argument arg;
    arg.write_value = [&x](const std::string &s) { internal::absorb(x, s); };
    arg.type = internal::infer_argument_type(x);
    arg.metavar = internal::type_to_help(x);
    auto handle_parse_argument = wahl::internal::overload{
        [&](const std::string &name) { arg.flags.push_back(name); },
        [&, this](auto &&attribute) -> decltype(attribute(x, *this, arg),
                                                void()) {
          attribute(x, *this, arg);
        },
    };
    wahl::internal::each_arg(handle_parse_argument, std::forward<Ts>(xs)...);
    this->add(std::move(arg));
  }

  argument &operator[](const std::string &flag) {
    if (lookup.find(flag) == lookup.end()) {
      const auto current_name =
          internal::try_name<std::tuple_element_t<0, std::tuple<Args...>>>();
      throw std::runtime_error(current_name + ": unknown flag: " + flag);
    }
    // else
    return arguments[lookup.at(flag)];
  }

  const argument &operator[](const std::string &flag) const {
    if (lookup.find(flag) == lookup.end()) {
      const auto current_name =
          internal::try_name<std::tuple_element_t<0, std::tuple<Args...>>>();
      throw std::runtime_error(current_name + ": unknown flag: " + flag);
    }
    // else
    return arguments[lookup.at(flag)];
  }

  void show_help_col(std::string item, std::string help, int width,
                     int total_width) const {
    auto txt = wahl::wrap(help, total_width - width - 2);
    assert(!txt.empty());
    std::cout << " " << std::setw(width) << item << " " << txt[0] << std::endl;
    std::for_each(txt.begin() + 1, txt.end(), [&](std::string line) {
      std::cout << " " << std::setw(width) << " "
                << " " << line << std::endl;
    });
  }

  void show_help(std::string name, std::string description,
                 std::string options_metavar) const {
    const int total_width = 80;
    std::vector<std::string> flags;
    int width = 0;
    for (auto &&arg : arguments) {
      std::string flag = arg.get_flags();
      width = std::max(width, int(flag.size()));
      flags.push_back(std::move(flag));
    }
    for (auto &&p : subcommands)
      width = std::max(width, int(p.first.size()));
    std::cout << "Usage: " << name << " " << options_metavar;

    if (subcommands.size() > 0)
      std::cout << " [command]";
    if (lookup.count("") > 0)
      std::cout << " " << (*this)[""].metavar;

    std::cout << std::endl;
    std::cout << std::endl;
    for (auto line : wahl::wrap(description, total_width - 2))
      std::cout << "  " << line << std::endl;
    std::cout << std::endl;
    std::cout << "Options: " << std::endl << std::endl;
    // TODO: Switch to different format when width > 40
    for (auto &&arg : arguments) {
      this->show_help_col(arg.get_flags(), arg.help, width, total_width);
    }
    if (subcommands.size() > 0) {
      std::cout << std::endl;
      std::cout << "Commands: " << std::endl << std::endl;
      for (auto &&p : subcommands) {
        this->show_help_col(p.first, p.second.help, width, total_width);
      }
    }
    std::cout << std::endl;
  }

  void post_process() {
    for (auto &&arg : arguments) {
      for (auto &&f : arg.callbacks)
        f(arg);
    }
  }
};

} // namespace wahl
