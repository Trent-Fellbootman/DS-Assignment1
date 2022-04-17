#include "application.hpp"
#include <iostream>
#include <sstream>
#define BUFFER_SIZE 1024

namespace app {

enum class OpType { ASSIGN, DISPLAY, EVALUATE, PLOT, UNKNOWN};

namespace helper {

OpType checkType(const char* buffer, char delimiter) {

}
}

template <typename T> void Application<T>::run() {
  char buffer[BUFFER_SIZE];
  while (true) {
    std::cin.getline(buffer, BUFFER_SIZE);
    std::string input(buffer, buffer + BUFFER_SIZE);
  }
}
} // namespace app