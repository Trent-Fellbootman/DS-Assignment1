#include "constants.h"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <stdint.h>
#include <string>
// only one warning/error is allowed per line

namespace app {
class Logger {
private:
  uint32_t currentIndent;
  std::string SGR_normal, SGR_warning, SGR_error, SGR_debug, SGR_info,
      SGR_output;
  bool indented;
  std::ostream &stream;
  int lineCount = 0;

public:
  enum class Alignment { LEFT, RIGHT, CENTER };
  enum class Level { NORMAL, WARNING, ERROR, DEBUG, INFO };

private:
  Level level;
  std::string *SGR_current;

  void checkIndent() {
    if (!indented) {
      // pad
      for (int i = 0; i < currentIndent; i++) {
        stream << WHITE_SPACE;
      }

      // check level
      switch (level) {
      case Level::WARNING:
        stream << LOGGER_PREFIX_WARNING;
        break;

      case Level::ERROR:
        stream << LOGGER_PREFIX_ERROR;
        break;
      case Level::DEBUG:
        stream << LOGGER_PREFIX_DEBUG;
        break;
      case Level::INFO:
        stream << LOGGER_PREFIX_INFO;
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

  void setSGR_output(std::string newColor) {
    SGR_output = newColor;
  }

  uint32_t getIndent() { return currentIndent; }
  void setIndent(uint32_t newIndent) { currentIndent = newIndent; }

  Level getLevel() { return level; }

  void setLevel(Level newLevel) {
    level = newLevel;

    // set color
    switch (level) {
    case Level::NORMAL:
      SGR_current = &SGR_output;
      break;

    case Level::WARNING:
      SGR_current = &SGR_warning;
      break;

    case Level::ERROR:
      SGR_current = &SGR_error;
      break;

    case Level::DEBUG:
      SGR_current = &SGR_debug;
      break;

    case Level::INFO:
      SGR_current = &SGR_info;
      break;
    }
  }

  Logger(std::ostream *stream = &std::cout)
      : stream(*stream), currentIndent(0), indented(false),
        level(Level::NORMAL), SGR_normal(SGR_FG_GREY),
        SGR_warning(SGR_FG_YELLOW), SGR_error(SGR_FG_RED),
        SGR_debug(SGR_FG_GREEN), SGR_info(SGR_FG_WHITE),
        SGR_output(SGR_FG_GREEN) {
    setLevel(level);
  }

  template <typename T> void printNumber(T number, Format format) {
    stream << *SGR_current;
    checkIndent();
    std::string str = std::to_string(number);
    // std::cout << str << std::endl;

    switch (format.alignment) {
    case Alignment::LEFT:
      stream << str;
      for (int i = 0; i < format.width - str.length(); i++) {
        stream << WHITE_SPACE;
      }
      break;

    case Alignment::RIGHT:
      for (int i = 0; i < format.width - str.length(); i++) {
        stream << WHITE_SPACE;
      }
      stream << str;
      break;

    case Alignment::CENTER:
      int padding = (format.width - str.length()) / 2;
      for (int i = 0; i < padding; i++) {
        stream << WHITE_SPACE;
      }
      stream << str;
      for (int i = 0; i < format.width - padding - str.length(); i++) {
        stream << WHITE_SPACE;
      }
      break;
    }
    stream << SGR_normal;
  }

  void printString(const std::string &str) {
    stream << *SGR_current;
    checkIndent();
    stream << str;
    stream << SGR_normal;
  }

  void endLine() {
    stream << std::endl;
    indented = false;
    lineCount++;
  }

  void println(const std::string &str) {
    stream << *SGR_current;
    printString(str);
    endLine();
    stream << SGR_normal;
  }

  void putchar(char ch, std::string SGR_color = "") {
    stream << *SGR_current;
    checkIndent();
    if (SGR_color != "") {
      stream << SGR_color;
    }
    stream << ch << SGR_normal;
  }

  void pad(uint32_t length, char ch = WHITE_SPACE) {
    stream << *SGR_current;
    checkIndent();
    for (int i = 0; i < length; i++) {
      stream << ch;
    }
    stream << SGR_normal;
  }

  void clear() {
    stream << ANSI_CLEAR_CONSOLE;
  }
};
} // namespace app