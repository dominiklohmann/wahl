// SPDX-License-Identifier: BSL-1.0

#include <args/args.hpp>
#include <doctest/doctest.h>

struct count_cmd {
  size_t verbose = 0;
  bool flag = false;

  template <class F> void parse(F f) {
    f(verbose, "-v", "--verbose", args::count());
    f(verbose, "-q", "--quiet", args::set(0));
    f(flag, "-f", "--flag", args::set(true));
  }

  void run() {}
};

TEST_CASE("count command") {
  auto cmd = count_cmd{};
  REQUIRE_EQ(cmd.verbose, 0);
  REQUIRE_FALSE(cmd.flag);

  SUBCASE("long options specified once") {
    args::parse(cmd, {"--verbose", "--flag"});
    CHECK_EQ(cmd.verbose, 1);
    CHECK(cmd.flag);
  }

  SUBCASE("short options specified once") {
    args::parse(cmd, {"-v", "-f"});
    CHECK_EQ(cmd.verbose, 1);
    CHECK(cmd.flag);
  }

  SUBCASE("short and long options mixes (1)") {
    args::parse(cmd, {"--verbose", "-f"});
    CHECK_EQ(cmd.verbose, 1);
    CHECK(cmd.flag);
  }

  SUBCASE("short and long options mixes (2)") {
    args::parse(cmd, {"-v", "--flag"});
    CHECK_EQ(cmd.verbose, 1);
    CHECK(cmd.flag);
  }

  SUBCASE("short options repeated twice without space") {
    args::parse(cmd, {"-vv", "--flag"});
    CHECK_EQ(cmd.verbose, 2);
    CHECK(cmd.flag);
  }

  SUBCASE("short options repeated thrice without space") {
    args::parse(cmd, {"-vvv", "--flag"});
    CHECK_EQ(cmd.verbose, 3);
    CHECK(cmd.flag);
  }

  SUBCASE("short options and flags interleaved (1)") {
    args::parse(cmd, {"-vf"});
    CHECK_EQ(cmd.verbose, 1);
    CHECK(cmd.flag);
  }

  SUBCASE("short options and flags interleaved (2)") {
    args::parse(cmd, {"-fv"});
    CHECK_EQ(cmd.verbose, 1);
    CHECK(cmd.flag);
  }

  SUBCASE("short options and flags interleaved (3)") {
    args::parse(cmd, {"-vfv"});
    CHECK_EQ(cmd.verbose, 2);
    CHECK(cmd.flag);
  }

  SUBCASE("short options and flags interleaved (4)") {
    args::parse(cmd, {"-vvf"});
    CHECK_EQ(cmd.verbose, 2);
    CHECK(cmd.flag);
  }

  SUBCASE("short option and long option") {
    args::parse(cmd, {"-v", "--verbose"});
    CHECK_EQ(cmd.verbose, 2);
    CHECK_FALSE(cmd.flag);
  }

  SUBCASE("repeated short option and long option") {
    args::parse(cmd, {"-vv", "--verbose"});
    CHECK_EQ(cmd.verbose, 3);
    CHECK_FALSE(cmd.flag);
  }

  SUBCASE("set callback sets a fixed value") {
    args::parse(cmd, {"--flag", "-q"});
    CHECK_EQ(cmd.verbose, 0);
    CHECK(cmd.flag);
  }
}
