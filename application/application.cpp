#include "application.hpp"
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#define BUFFER_SIZE 1024

#define LEFT_BRACE '('
#define RIGHT_BRACE ')'
#define WHITE_SPACE ' '
#define LEFT_BRACKET '{'
#define RIGHT_BRACKET '}'

#define PLUS_CH '+'
#define MINUS_CH '-'
#define MULTIPLY_CH '*'
#define DIVIDE_CH '/'

#define OP_ASSIGN "assign"
#define OP_DISPLAY "disp"
#define OP_EVALUATE "eval"
#define OP_PLOT "plot"
#define OP_HELP "help"
#define OP_EXIT "exit"

#define INVALID_COMMAND_MESSAGE "Command is invalid (Type \"help()\" for help)."
#define HELPER_MESSAGE "TODO"
#define UNKNOWN_OP_MESSAGE "TODO"

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

enum class TokenTypes { VAR, POLY, OP };

enum class CalcOps { ADD, SUB, MUL, DIV };

template <typename T> union TokenData {
  std::string variable;
  poly::Polynomial<T> polynomial;
  CalcOps operation;
};

template <typename T> struct Token {
  TokenTypes tp;
  TokenData<T> data;
};

std::string strip(std::string original, char ch) {
  while (original.find(ch) != std::string::npos) {
    original.erase(original.find(ch));
  }
  return std::move(original);
}

std::vector<std::string> separate(std::string original,
                                  std::set<char> delimiters) {
  std::vector<std::string> res;

  int last = 0;

  while (true) {
    bool found = false;
    for (char delimiter : delimiters) {
      int tmp = original.find_first_of(delimiter, last);
      if (tmp != std::string::npos) {
        if (last < tmp) {
          res.push_back(
              std::string(original.begin() + last, original.begin() + tmp));
        }
        res.push_back(
            std::string(original.begin() + tmp, original.begin() + tmp + 1));
        last = tmp + 1;
        found = true;
      }
    }

    if (!found) {
      if (last < original.size()) {
        res.push_back(std::string(original.begin() + last, original.end()));
      }
      break;
    }
  }

  return std::move(res);
}

template <typename T>
std::vector<Token<T>> expressionToTokens(std::string expr) {
  std::string stripped = strip(expr, WHITE_SPACE);
  std::set<char> delimiters{LEFT_BRACE,    RIGHT_BRACE, LEFT_BRACKET,
                            RIGHT_BRACKET, PLUS_CH,     MINUS_CH,
                            MULTIPLY_CH,   DIVIDE_CH};
  std::vector<std::string> separated = separate(stripped, delimiters);

  
}

} // namespace helper

template <typename T> void Application<T>::run() {
  char buffer[BUFFER_SIZE];
  while (true) {
    std::cin.getline(buffer, BUFFER_SIZE);
    std::string input(buffer, buffer + BUFFER_SIZE);

    if (std::count(input.begin(), input.end(), LEFT_BRACE) == 0 ||
        std::count(input.begin(), input.end(), RIGHT_BRACE) == 0) {
      std::cout << INVALID_COMMAND_MESSAGE << std::endl;
      continue;
    }

    int index = input.find_first_of(LEFT_BRACE);
    OpType operation = helper::stringToType(buffer, index);

    switch (operation) {
    case OpType::HELP: {
      std::cout << HELPER_MESSAGE << std::endl;
      continue;
    } break;

    case OpType::ASSIGN: {

    } break;

    case OpType::DISPLAY: {
      printf("Found %d polynomials:\n", polynomials.size());
      for (auto &pair : polynomials) {
        printf("%s(x) = %s\n", pair.first, pair.second.format());
      }
    } break;

    case OpType::EVALUATE: {

    } break;

    case OpType::PLOT: {

    } break;

    case OpType::UNKNOWN: {
      std::cout << UNKNOWN_OP_MESSAGE;
    } break;

    case OpType::EXIT: {
      return;
    }

    default:
      break;
    }
  }
}

template <typename T>
std::unique_ptr<poly::Polynomial<T>>
Application<T>::evaluateExpression(std::string expr) {}

template <typename T>
void Application<T>::plot(std::string polyName, double start, double end) {
  std::vector<double> values(canvasExtent.width);
  double pace = canvasExtent.width / (canvasExtent.width - 1);
}
} // namespace app