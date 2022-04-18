#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <string>

namespace app {
class Logger {
private:
  uint32_t currentIndent;

  void disableBuffer() {
    std::cout << std::unitbuf;
  }

public:
  enum class Alignment { LEFT, RIGHT, CENTER };

  struct Format {
    Alignment alignment;
    uint32_t width;
  };

  uint32_t getIndent() { return currentIndent; }
  uint32_t setIndent(uint32_t newIndent) { currentIndent = newIndent; }

  void setFormat(Alignment alignment, uint32_t outputWidth,
                 uint32_t contentWidth = 0)
  // contentWidth is an optional parameter required with alignment is set to
  // center
  {
    switch (alignment) {
    case Alignment::LEFT:
      std::cout << std::ios::left << std::setw(outputWidth);
      break;

    case Alignment::RIGHT:
      std::cout << std::ios::right << std::setw(outputWidth);
      break;

    case Alignment::CENTER:
      uint32_t padding = (outputWidth - contentWidth) / 2;
      for (int i = 0; i < padding; i++) {
        std::cout << ' ';
      }
      std::cout << std::ios::left << std::setw(outputWidth - padding);
      break;

    default:
      break;
    }
  }

  void printString(const std::string &str) {
    for (int i = 0; i < currentIndent; i++) {
      std::cout << ' ';
    }
    std::cout << str;
  }

  void endLine() { std::cout << std::endl; }
};
} // namespace app