#include "constants.h"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <string>

// only one warning/error is allowed per line

namespace app {
class Logger {
private:
  uint32_t currentIndent;
  std::string SGR_normal = SGR_FG_GREY, SGR_warning = SGR_FG_YELLOW,
              SGR_error = SGR_FG_RED;
  bool indented = false;

public:
  enum class Alignment { LEFT, RIGHT, CENTER };
  enum class Level { NORMAL, WARNING, ERROR };

private:
  Level level = Level::NORMAL;
  std::string *SGR_current = &SGR_normal;

  void checkIndent() {
    if (!indented) {
      // pad
      for (int i = 0; i < currentIndent; i++) {
        std::cout << WHITE_SPACE;
      }

      // check level
      switch (level) {
      case Level::WARNING:
        std::cout << LOGGER_PREFIX_WARNING;
        break;

      case Level::ERROR:
        std::cout << LOGGER_PREFIX_ERROR;
        break;
      }
      indented = true;
    }
  }

public:
  struct Format {
    Alignment alignment;
    uint32_t width;
  };

  Logger() : currentIndent(0) {}

  uint32_t getIndent() { return currentIndent; }
  void setIndent(uint32_t newIndent) { currentIndent = newIndent; }

  Level getLevel() { return level; }

  void setLevel(Level newLevel) {
    level = newLevel;

    // set color
    switch (level) {
    case Level::NORMAL:
      SGR_current = &SGR_normal;
      std::cout << SGR_current;
      break;
    case Level::WARNING:
      SGR_current = &SGR_warning;
      std::cout << SGR_current;
      break;
    case Level::ERROR:
      SGR_current = &SGR_error;
      std::cout << SGR_current;
      break;
    }
  }

  template <typename T> void printNumber(T number, Format format) {
    checkIndent();
    std::string str = std::to_string(number);

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
    checkIndent();
    for (int i = 0; i < currentIndent; i++) {
      std::cout << WHITE_SPACE;
    }
  }

  void println(const std::string &str, Level level = Level::NORMAL) {
    printString(str, level);
    endLine();
  }

  void putchar(char ch, std::string SGR_color = "") {
    checkIndent();
    if (SGR_color != "") {
      std::cout << SGR_color;
    }
    std::cout << ch << SGR_current;
  }

  void endLine() { std::cout << std::endl; }

  void pad(uint32_t length, char ch = WHITE_SPACE) {
    checkIndent();
    for (int i = 0; i < length; i++) {
      std::cout << ch;
    }
  }
};
} // namespace app