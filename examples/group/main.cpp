// SPDX-License-Identifier: BSL-1.0

#include <wahl/wahl.hpp>

struct cli : wahl::group<cli> {
  static const char *help() {
    return "Command-line interface to manage a database";
  }
};

struct initdb : cli::command<initdb> {
  static const char *help() { return "Initialize database"; }
  void run() { printf("Initialize database\n"); }
};

struct dropdb : cli::command<dropdb> {
  static const char *help() { return "Delete database"; }
  void run() { printf("Delete database\n"); }
};

int main(int argc, const char **argv) { wahl::parse<cli>(argc, argv); }
