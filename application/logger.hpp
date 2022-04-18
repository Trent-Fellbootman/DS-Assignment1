#include "constants.h"
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <string>

namespace app {
class Logger {
private:
  uint32_t currentIndent;
  std::string SGR_normal = SGR_FG_GREY, SGR_warning = SGR_FG_YELLOW, SGR_error = SGR_FG_RED;

public:
  enum class Alignment { LEFT, RIGHT, CENTER };
  enum class Level { NORMAL, WARNING, ERROR };

  struct Format {
    Alignment alignment;
    uint32_t width;
  };

  Logger() : currentIndent(0) {}

  uint32_t getIndent() { return currentIndent; }
  void setIndent(uint32_t newIndent) { currentIndent = newIndent; }

  template <typename T> void printNumber(T number, Format format) {
    std::string str = std::to_string(number);
    for (int i = 0; i < currentIndent; i++) {
      std::cout << WHITE_SPACE;
    }

    switch (format.alignment) {
    case Alignment::LEFT:
      std::cout << std::ios::left << std::setw(format.width) << str;
      break;

    case Alignment::RIGHT:
      std::cout << std::ios::right << std::setw(format.width) << str;
      break;

    case Alignment::CENTER:
      int padding = (format.width - str.length()) / 2;
      for (int i = 0; i < padding; i++) {
        std::cout << WHITE_SPACE;
      }
      std::cout << std::setw(format.width - padding) << str;
      break;
    }
  }

  void printString(const std::string &str, Level level = Level::NORMAL) {
    for (int i = 0; i < currentIndent; i++) {
      std::cout << WHITE_SPACE;
    }

    switch (level) {
      case Level::NORMAL:
        std::cout << SGR_normal << str << SGR_normal;
        break;
      case Level::WARNING:
        std::cout << SGR_warning << LOGGER_PREFIX_WARNING << str << SGR_normal;
        break;
      case Level::ERROR:
        std::cout << SGR_error << LOGGER_PREFIX_ERROR << str << SGR_normal;
        break;
    }
  }

  void println(const std::string &str, Level level = Level::NORMAL) {
    printString(str, level);
    endLine();
  }

  void endLine() { std::cout << std::endl; }
};
} // namespace app