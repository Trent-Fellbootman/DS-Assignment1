#include "application.hpp"
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#define BUFFER_SIZE 1024
#define LEFT_BRACE '('
#define RIGHT_BRACE ')'
#define OP_ASSIGN "assign"
#define OP_DISPLAY "disp"
#define OP_EVALUATE "eval"
#define OP_PLOT "plot"
#define OP_HELP "help"
#define OP_EXIT "exit"

#define INVALID_COMMAND "Command is invalid (Type \"help()\" for help)."

namespace app {

enum class OpType { ASSIGN, DISPLAY, EVALUATE, PLOT, UNKNOWN, HELP, EXIT };

namespace helper {

OpType stringToType(char *buffer, int length) {
  char tmp = buffer[length];
  buffer[length] = '\0';
  OpType res;
  if (strcmp(buffer, OP_ASSIGN) == 0) {
    res = OpType::ASSIGN;
  } else if (strcmp(buffer, OP_DISPLAY) == 0) {
    res = OpType::DISPLAY;
  } else if (strcmp(buffer, OP_EVALUATE) == 0) {
    res = OpType::EVALUATE;
  } else if (strcmp(buffer, OP_PLOT) == 0) {
    res = OpType::PLOT;
  } else if (strcmp(buffer, OP_HELP) == 0) {
    res = OpType::HELP;
  } else if (strcmp(buffer, OP_EXIT) == 0) {
    res = OpType::EXIT;
  } else {
    res = OpType::UNKNOWN;
  }
  buffer[length] = tmp;
  return res;
}
} // namespace helper

template <typename T> void Application<T>::run() {
  char buffer[BUFFER_SIZE];
  while (true) {
    std::cin.getline(buffer, BUFFER_SIZE);
    std::string input(buffer, buffer + BUFFER_SIZE);

    if (std::count(input.begin(), input.end(), LEFT_BRACE) == 0 ||
            std::count(input.begin(), input.end(), RIGHT_BRACE) == 0) {
      std::cout << INVALID_COMMAND << std::endl;
      continue;
    }

    int index = input.find_first_of(LEFT_BRACE);
    OpType operation = helper::stringToType(buffer, index);

    switch (operation) {
    case OpType::HELP: {

    } break;

    case OpType::ASSIGN: {
      
    } break;

    case OpType::DISPLAY: {
      
    } break;

    case OpType::EVALUATE: {
      
    } break;

    case OpType::PLOT: {
      
    } break;

    case OpType::UNKNOWN: {
      
    } break;

    default:
      break;
    }
  }
}
} // namespace app