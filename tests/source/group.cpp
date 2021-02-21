// SPDX-License-Identifier: BSL-1.0

#include <wahl/wahl.hpp>
#include <doctest/doctest.h>

struct cli : wahl::group<cli> {
  std::string name = "";
};

struct init : cli::command<init> {
  init() {}
  void run(cli &c) { c.name = "init"; }
};

struct delete_ : cli::command<delete_> {
  delete_() {}
  void run(cli& c) {
    c.name = "delete";
  }
};

struct custom_name : cli::command<custom_name> {
  static const char *name() { return "run"; }
  custom_name() {}
  void run(cli &c) { c.name = "run"; }
};

TEST_CASE("command groups") {
  auto cmd = cli{};
  REQUIRE_EQ(cmd.name, "");

  SUBCASE("command with automatic name") {
    wahl::parse(cmd, {"init"});
    CHECK_EQ(cmd.name, "init");
  }

  SUBCASE("command with manual name") {
    wahl::parse(cmd, {"run"});
    CHECK_EQ(cmd.name, "run");
  }

  SUBCASE("command with automatic name with trailing underscore") {
    wahl::parse(cmd, {"delete"});
    CHECK_EQ(cmd.name, "delete");
  }
}
