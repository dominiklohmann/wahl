// SPDX-License-Identifier: BSL-1.0

#pragma once

#include <wahl/argument.hpp>
#include <wahl/argument_type.hpp>
#include <wahl/context.hpp>
#include <wahl/internal/absorb.hpp>
#include <wahl/internal/each_arg.hpp>
#include <wahl/internal/is_container.hpp>
#include <wahl/internal/overload.hpp>
#include <wahl/internal/rank.hpp>
#include <wahl/internal/try_help.hpp>
#include <wahl/internal/try_name.hpp>
#include <wahl/internal/try_options_metavar.hpp>
#include <wahl/internal/try_parse.hpp>
#include <wahl/internal/type_name.hpp>
#include <wahl/internal/type_to_help.hpp>
#include <wahl/subcommand.hpp>
#include <wahl/value_parser.hpp>
#include <wahl/version.hpp>

#include <algorithm>
#include <deque>
#include <functional>
#include <initializer_list>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <iomanip>
#include <iostream>

#include <cassert>

#if defined(_MSC_VER)
#include <ciso646>
#endif

namespace wahl {

template <class F> auto callback(F f) {
  return [f](auto &&data, auto &ctx, argument &a) {
    a.add_callback(
        [f, &ctx, &data](const argument &arg) { f(data, ctx, arg); });
  };
}

template <class F> auto eager_callback(F f) {
  return [f](auto &&data, auto &ctx, argument &a) {
    a.add_eager_callback(
        [f, &data, &ctx](const argument &arg) { f(data, ctx, arg); });
  };
}

template <class F> auto lazy_callback(F f) {
  return [f](auto &&data, auto &ctx, argument &a) {
    a.add_callback([f, &ctx, &data](const argument &arg) {
      if (arg.count > 0)
        f(data, ctx, arg);
    });
  };
}

template <class F> auto action(F f) {
  return wahl::eager_callback(std::bind(f));
}

template <class T> auto show(T text) {
  return action([=] { std::cout << text << std::endl; });
}

inline auto required() {
  return [](auto &&, auto &, argument &a) {
    a.required = true;
    a.add_callback([](const argument &arg) {
      if (arg.required and arg.count == 0)
        throw std::runtime_error("required arg missing: " + arg.get_flags());
    });
  };
}

template <class T> auto set(T value) {
  return [value](auto &&data, auto &, argument &a) {
    a.type = argument_type::none;
    a.write_value = [&data, value](const std::string &) { data = value; };
  };
}

inline auto count() {
  return [](auto &&data, auto &, argument &a) {
    a.type = argument_type::none;
    a.add_callback([&data](const argument &arg) { data = arg.count; });
    a.write_value = [](const std::string &) {};
  };
}

template <class T> auto help(T &&x) {
  return [=](auto &&, auto &, argument &a) { a.help = x; };
};

template <class T> auto metavar(T &&x) {
  return [=](auto &&, auto &, argument &a) { a.metavar = x; };
};

template <class C, class T>
auto assign_subcommands(internal::rank<1>, C &ctx, T &)
    -> decltype(T::subcommands, void()) {
  ctx.subcommands = T::subcommands();
}

template <class C, class T>
void assign_subcommands(internal::rank<0>, C &, T &) {}

template <class... Ts, class T> context<T &, Ts...> build_context(T &cmd) {
  context<T &, Ts...> ctx;
  wahl::assign_subcommands(internal::rank_v<1>, ctx, cmd);
  ctx.parse(
      nullptr, "-h", "--help", wahl::help("Show help"),
      wahl::eager_callback([](std::nullptr_t, const auto &c, const argument &) {
        c.show_help(internal::try_name<T>(), internal::try_help<T>(),
                    internal::try_options_metavar<T>());
      }));
  wahl::internal::try_parse(
      cmd, [&](auto &&...xs) { ctx.parse(std::forward<decltype(xs)>(xs)...); });
  return ctx;
}

template <class Iterator>
std::string pop_string(Iterator first, Iterator last) {
  if (first == last)
    return std::string();
  else
    return std::string(first + 1, last);
}

inline std::tuple<std::string, std::string>
parse_attached_value(const std::string &s) {
  assert(s.size() > 0);
  assert(s[0] == '-' && "Not parsing a flag");
  if (s[1] == '-') {
    auto it = std::find(s.begin(), s.end(), '=');
    return std::make_tuple(std::string(s.begin(), it),
                           wahl::pop_string(it, s.end()));
  } else if (s.size() > 2) {
    return std::make_tuple(s.substr(0, 2), s.substr(2));
  } else {
    return std::make_tuple(s, std::string());
  }
}

template <class T, class... Ts>
auto try_run(internal::rank<2>, T &x, Ts &&...xs) -> decltype(x.run(xs...)) {
  return (x.run(xs...));
};

template <class T, class... Ts>
auto try_run(internal::rank<1>, T &x, Ts &&...) -> decltype(x.run()) {
  return (x.run());
}

template <class T, class... Ts>
void parse(T &cmd, std::deque<std::string> a, Ts &&...xs) {
  auto ctx = wahl::build_context<Ts...>(cmd);

  bool capture = false;
  std::string core;
  for (auto &&x : a) {
    if (ctx.has_subcommand(x)) {
      a.pop_front();
      ctx.subcommands[x].run(a, cmd, xs...);
      return;
    }
    if (x[0] == '-') {
      capture = false;
      std::string value;
      std::tie(core, value) = wahl::parse_attached_value(x);

      if (ctx[core].type == argument_type::none) {
        if (ctx[core].write(""))
          return;
        for (auto &&c : value)
          if (ctx[std::string("-") + c].write(""))
            return;
      } else if (not value.empty()) {
        ctx[core].write(value);
      } else {
        capture = true;
      }
    } else if (capture) {
      if (ctx[core].write(x))
        return;
      capture = ctx[core].type == argument_type::multiple;
    } else if (ctx.has_default_capture()) {
      ctx[""].write(x);
    } else {
      if (core.empty()) {
        throw std::runtime_error("unknown command: " + x);
      }
      if (ctx[core].type == argument_type::none) {
        throw std::runtime_error("flag: " + core +
                                 " does not expect an argument.");
      }
      if (ctx[core].type != argument_type::multiple) {
        throw std::runtime_error("flag: " + core +
                                 " expects only one argument.");
      }
    }

    a.pop_front();
  }
  ctx.post_process();

  wahl::try_run(internal::rank_v<2>, cmd, xs...);
}

template <class T, class... Ts>
void parse(std::deque<std::string> a, Ts &&...xs) {
  T cmd = {};

  wahl::parse(cmd, std::move(a), xs...);
}

template <class T> bool parse(int argc, char const *argv[]) {
  std::deque<std::string> as(argv + 1, argv + argc);

  try {
    wahl::parse<T>(as);
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return false;
  }

  return true;
}

template <class T, class F> struct auto_register {
  static bool auto_register_reg_;
  static bool auto_register_reg_init_() {
    F::template apply<T>();
    return true;
  }

  auto_register() { (void)auto_register_reg_; }
};

template <class T, class F>
bool auto_register<T, F>::auto_register_reg_ =
    auto_register<T, F>::auto_register_reg_init_();

template <class Derived> struct group {
  using context_type = context<Derived &>;
  using subcommand_type = typename context_type::subcommand_type;
  using subcommand_map = typename context_type::subcommand_map;

  static subcommand_map &subcommands() {
    static subcommand_map subcommands_;
    return subcommands_;
  }

  template <class T> static void add_command() {
    subcommand_type sub;
    sub.run = [](auto a, auto &&...xs) { wahl::parse<T>(a, xs...); };
    sub.help = internal::try_help<T>();
    subcommands().emplace(internal::try_name<T>(), sub);
  }

  struct auto_register_command {
    template <class T> static void apply() { add_command<T>(); }
  };

  template <class D>
  struct command : auto_register<D, auto_register_command> {};

  void run() {}
};

} // namespace wahl
