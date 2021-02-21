<p align="center">
    <h1 align="center">wahl</h1>
    <p align="center">A type-safe argument parser for modern C++.</p>
</p>

## Quick Start

Simply provide a class with fields that is to be filled by command-line
arguments:

```c++
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

int main(int argc, const char **argv) {
  wahl::parse<hello>(argc, argv);
}
```



## Acknowledgements

This project is derived from [pfultz2/args][pfultz2/args], and I want to thank
[Paul Fultz II][pfultz2] and [Daniël Emmery][Dmry] for the idea and their work
on the original library.

[pfultz2/args]: https://github.com/pfultz2/args
[pfultz2]: https://github.com/pfultz2
[Dmry]: https://github.com/Dmry
