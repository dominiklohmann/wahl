// SPDX-License-Identifier: BSL-1.0

#include <wahl/wahl.hpp>
#include <doctest/doctest.h>

struct callback_cmd {
  size_t count = 1;
  std::string name = {};
  std::string final_name = {};

  template <class F> void parse(F f) {
    auto cb = [this](auto &&data, const auto &, const wahl::argument &) {
      for (size_t i = 0; i < data; ++i)
        final_name += name;
    };
    f(count, "--count", "-C", wahl::callback(cb));
    f(name, "--name", "-N");
  }

  void run() {}
};

TEST_CASE("command callbacks") {
  auto cmd = callback_cmd{};
  REQUIRE_EQ(cmd.count, 1);
  REQUIRE_EQ(cmd.name, "");
  REQUIRE_EQ(cmd.final_name, "");

  SUBCASE("callback with all options speciied") {
    wahl::parse(cmd, {"--count", "5", "--name", "hello"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "hello");
    CHECK_EQ(cmd.final_name, "hellohellohellohellohello");
  }

  SUBCASE("callbacks are evaluated for omitted options") {
    wahl::parse(cmd, {"--name", "hello"});
    CHECK_EQ(cmd.count, 1);
    CHECK_EQ(cmd.name, "hello");
    CHECK_EQ(cmd.final_name, "hello");
  }
}

struct lazy_callback_cmd {
  size_t count = 1;
  std::string name;
  std::string final_name;

  template <class F> void parse(F f) {
    auto cb = [this](auto &&data, const auto &, const wahl::argument &) {
      for (size_t i = 0; i < data; ++i)
        final_name += name;
    };
    f(count, "--count", "-C", wahl::lazy_callback(cb));
    f(name, "--name", "-N");
  }

  void run() {}
};

TEST_CASE("lazy command callbacks") {
  auto cmd = lazy_callback_cmd{};
  REQUIRE_EQ(cmd.count, 1);
  REQUIRE_EQ(cmd.name, "");
  REQUIRE_EQ(cmd.final_name, "");

  SUBCASE("lazy callback with all options speciied") {
    wahl::parse(cmd, {"--count", "5", "--name", "hello"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "hello");
    CHECK_EQ(cmd.final_name, "hellohellohellohellohello");
  }

  SUBCASE("lazy callbacks are not evaluated for omitted options") {
    wahl::parse(cmd, {"--name", "hello"});
    CHECK_EQ(cmd.count, 1);
    CHECK_EQ(cmd.name, "hello");
    CHECK_EQ(cmd.final_name, "");
  }
}

struct lazy_callback_null_cmd {
  size_t count = 0;
  std::string name = "";
  std::string final_out = "";

  template <class F> void parse(F f) {
    auto cb = [this](auto &&, const auto &, const wahl::argument &) {
      for (size_t i = 0; i < count; ++i)
        final_out += name;
    };
    f(nullptr, "--out", "-O", wahl::lazy_callback(cb));
    f(count, "--count", "-C");
    f(name, "--name", "-N");
  }

  void run() {}
};

TEST_CASE("lazy command callbacks with null options") {
  auto cmd = lazy_callback_null_cmd{};
  REQUIRE_EQ(cmd.count, 0);
  REQUIRE_EQ(cmd.name, "");
  REQUIRE_EQ(cmd.final_out, "");

  SUBCASE("lazy callback with all options speciied") {
    wahl::parse(cmd, {"--out", "--count", "5", "--name", "hello"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "hello");
    CHECK_EQ(cmd.final_out, "hellohellohellohellohello");
  }

  SUBCASE("lazy callbacks are not evaluated for omitted options") {
    wahl::parse(cmd, {"--count", "5", "--name", "hello"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "hello");
    CHECK_EQ(cmd.final_out, "");
  }
}

struct eager_callback_cmd {
  size_t count = 0;
  std::string name = "";
  std::string final_out = "";

  template <class F> void parse(F f) {
    auto cb = [this](auto &&data, const auto &, const wahl::argument &) {
      final_out = std::to_string(data);
    };
    f(count, "--count", "-C", wahl::eager_callback(cb));
    f(name, "--name", "-N");
  }

  void run() {}
};

TEST_CASE("eager command callbacks") {
  auto cmd = eager_callback_cmd{};
  REQUIRE_EQ(cmd.count, 0);
  REQUIRE_EQ(cmd.name, "");
  REQUIRE_EQ(cmd.final_out, "");

  SUBCASE("options after the eager callback are not evaluated") {
    wahl::parse(cmd, {"--count", "5", "--name", "hello"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "");
    CHECK_EQ(cmd.final_out, "5");
  }

  SUBCASE("options up until the eager callback are evaluated") {
    wahl::parse(cmd, {"--name", "hello", "--count", "5"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "hello");
    CHECK_EQ(cmd.final_out, "5");
  }
}

struct action_null_cmd {
  size_t count = 0;
  std::string name = "";
  std::string final_out = "";

  template <class F> void parse(F f) {
    f(nullptr, "--out", "-O", wahl::action([this] { final_out = "out"; }));
    f(count, "--count", "-C");
    f(name, "--name", "-N");
  }

  void run() {}
};

TEST_CASE("command actions") {
  auto cmd = action_null_cmd{};
  REQUIRE_EQ(cmd.count, 0);
  REQUIRE_EQ(cmd.name, "");
  REQUIRE_EQ(cmd.final_out, "");

  SUBCASE("actions are eager callbacks without a parameter") {
    wahl::parse(cmd, {"--out", "--count", "5", "--name", "hello"});
    CHECK_EQ(cmd.count, 0);
    CHECK_EQ(cmd.name, "");
    CHECK_EQ(cmd.final_out, "out");
  }
}
