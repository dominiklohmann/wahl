// SPDX-License-Identifier: BSL-1.0

#include <wahl/wahl.hpp>

#include <sstream>

namespace wahl {

std::vector<std::string> wrap(const std::string &text,
                              unsigned int line_length) {
  std::vector<std::string> output;
  std::istringstream iss(text);

  std::string line;

  do {
    std::string word;
    iss >> word;

    if (line.length() + word.length() > line_length) {
      output.push_back(line);
      line.clear();
    }
    line += word + " ";

  } while (iss);

  if (!line.empty()) {
    output.push_back(line);
  }
  return output;
}

} // namespace wahl
