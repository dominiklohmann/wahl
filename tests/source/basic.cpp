// SPDX-License-Identifier: BSL-1.0

#include <args/args.hpp>
#include <doctest/doctest.h>

struct single_value_cmd {
  size_t count = 0;
  std::string name = "";

  template <class F> void parse(F f) {
    f(count, "--count", "-C");
    f(name, "--name", "-N");
  }

  void run() {}
};

TEST_CASE("single value command") {
  auto cmd = single_value_cmd{};
  REQUIRE_EQ(cmd.count, 0);
  REQUIRE_EQ(cmd.name, "");

  SUBCASE("log-options with space") {
    args::parse(cmd, {"--count", "5", "--name", "hello"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "hello");
  }

  SUBCASE("options may be omitted") {
    args::parse(cmd, {"--count", "5"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "");
  }

  SUBCASE("long options with equal-sign") {
    args::parse(cmd, {"--count=5", "--name=hello"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "hello");
  }

  SUBCASE("short options with space") {
    args::parse(cmd, {"-C", "5", "-N", "hello"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "hello");
  }

  SUBCASE("short options without space") {
    args::parse(cmd, {"-C5", "-Nhello"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.name, "hello");
  }
}

struct multi_value_cmd {
  size_t count = 0;
  std::vector<std::string> names = {};

  template <class F> void parse(F f) {
    f(count, "--count", "-C");
    f(names, "--name", "-N");
  }

  void run() {}
};

TEST_CASE("multi value command") {
  auto cmd = multi_value_cmd{};
  REQUIRE_EQ(cmd.count, 0);
  REQUIRE(cmd.names.empty());

  SUBCASE("repeated short options with spaces") {
    args::parse(cmd, {"-N", "1", "-N", "2", "-N", "3", "--count", "5"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }

  SUBCASE("single short option with spaces") {
    args::parse(cmd, {"-N", "1", "2", "3", "--count", "5"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }

  SUBCASE("repeated short options without spaces") {
    args::parse(cmd, {"-N1", "-N2", "-N3", "--count", "5"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }

  SUBCASE("repeated long options with spaces") {
    args::parse(cmd,
                {"--name", "1", "--name", "2", "--name", "3", "--count", "5"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }

  SUBCASE("single long option with spaces") {
    args::parse(cmd, {"--name", "1", "2", "3", "--count", "5"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }

  SUBCASE("repeated long options with equal-sign") {
    args::parse(cmd, {"--name=1", "--name=2", "--name=3", "--count", "5"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }
}

struct no_data_cmd {
  std::string name = "";

  template <class F> void parse(F f) {
    f(nullptr, "--null");
    f(name, "--name", "-N");
  }

  void run() {}
};

TEST_CASE("no value command") {
  auto cmd = no_data_cmd{};
  REQUIRE_EQ(cmd.name, "");
  args::parse(cmd, {"--null", "--name=hello"});
  CHECK_EQ(cmd.name, "hello");
}

struct positional_arguments_cmd {
  size_t count = 0;
  std::vector<std::string> names = {};

  template <class F> void parse(F f) {
    f(count, "--count", "-C");
    f(names);
  }

  void run() {}
};

TEST_CASE("positional arguments command") {
  auto cmd = positional_arguments_cmd{};
  REQUIRE_EQ(cmd.count, 0);
  REQUIRE(cmd.names.empty());

  SUBCASE("positional arguments before option") {
    args::parse(cmd, {"1", "2", "3", "--count", "5"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }

  SUBCASE("positional arguments after long option with space") {
    args::parse(cmd, {"--count", "5", "1", "2", "3"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }

  SUBCASE("positional arguments after long option with equal-sign") {
    args::parse(cmd, {"--count=5", "1", "2", "3"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }

  SUBCASE("positional arguments after short option with space") {
    args::parse(cmd, {"-C", "5", "1", "2", "3"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }

  SUBCASE("positional arguments after short option without space") {
    args::parse(cmd, {"-C5", "1", "2", "3"});
    CHECK_EQ(cmd.count, 5);
    CHECK_EQ(cmd.names.size(), 3);
    CHECK_EQ(cmd.names[0], "1");
    CHECK_EQ(cmd.names[1], "2");
    CHECK_EQ(cmd.names[2], "3");
  }
}

struct no_arguments_cmd {
  bool check = false;
  void run() { check = true; }
};

TEST_CASE("no arguments command") {
  auto cmd = no_arguments_cmd{};
  REQUIRE_FALSE(cmd.check);
  args::parse(cmd, {});
  CHECK(cmd.check);
}
