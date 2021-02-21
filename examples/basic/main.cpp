// SPDX-License-Identifier: BSL-1.0

#include <wahl/wahl.hpp>

struct hello {
  static const char *help() {
    return "Simple program that greets NAME for a total of COUNT times.";
  }

  int count;
  std::string name;

  hello() : count(1) {}

  template <class F> void parse(F f) {
    f(count, "--count", "-C", wahl::help("Number of greetings."));
    f(name, "--name", "-N", wahl::help("The person to greet."),
      wahl::required());
  }

  void run() {
    for (int i = 0; i < count; i++)
      printf("%s\n", name.c_str());
  }
};

int main(int argc, const char **argv) { wahl::parse<hello>(argc, argv); }
