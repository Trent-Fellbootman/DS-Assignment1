#include "constants.h"
#include "helper.hpp"
#include "polynomial.hpp"
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#define BUFFER_SIZE 1024

namespace app {
template <typename T> class Application {

public:
  Application() = default;
  void run();

private:
  struct Extent {
    uint32_t width;
    uint32_t height;
  };

  std::map<std::string, poly::Polynomial<T>> polynomials;

  void mainLoop();
  void plot(std::string polyName, double start, double end);

  std::unique_ptr<poly::Polynomial<T>> evaluateExpression(std::string expr);
  poly::Polynomial<T>
  calculateExpr(const std::vector<helper::Token<T>> &tokens);

  Extent canvasExtent{80, 24};
};
} // namespace app

namespace app {
template <typename T>
poly::Polynomial<T>
Application<T>::calculateExpr(const std::vector<helper::Token<T>> &tokens) {
  std::stack<poly::Polynomial<T>> ret;
  std::vector<helper::Token<T>> rpn = helper::expression2RPN(tokens);
  for (int i = 0; i < rpn.size(); i++) {
    if (rpn[i].tp == TokenTypes::POLY) {
      std::vector<std::pair<T, uint32_t>> debug = rpn[i].data.polynomial.dump();
      std::cout << "rpn dumped" << std::endl;
    }
  }
  for (int i = 0; i < rpn.size(); i++) {
    if (rpn[i].tp == TokenTypes::OP) {
      if (ret.size() < 1) {
        std::cout << "Invalid operation" << std::endl;
      } else if (ret.size() == 1) {
        poly::Polynomial<T> t = ret.top();
        ret.pop();
        switch (rpn[i].data.operation) {
        case CalcOps::ADD: {
          ret.push(t);
        } break;
        case CalcOps::SUB: {
          T scale = -1;
          ret.push(t * scale);
        } break;
        }
      } else {
        poly::Polynomial<T> v2 = ret.top();
        ret.pop();
        poly::Polynomial<T> v1 = ret.top();
        ret.pop();
        switch (rpn[i].data.operation) {
        case CalcOps::ADD: {
          ret.push(v1 + v2);
        } break;
        case CalcOps::SUB: {
          ret.push(v1 - v2);
        } break;
        case CalcOps::MUL: {
          std::cout << "Not implemented" << std::endl;
        } break;
        case CalcOps::DIV: {
          std::cout << "Not implemented" << std::endl;
        } break;
        }
      }

    } else if (rpn[i].tp == TokenTypes::VAR) {
      auto it = polynomials.find(rpn[i].data.variable);
      if (it == polynomials.end()) {
        printf("Can't find variable %s\n", rpn[i].data.variable);
      } else {
        ret.push(it->second);
      }

    } else if (rpn[i].tp == TokenTypes::POLY) {
      ret.push(rpn[i].data.polynomial);
    }
  }

  return ret.top();
}
template <typename T> void Application<T>::run() {
  char buffer[BUFFER_SIZE];
  while (true) {
    std::cin.getline(buffer, BUFFER_SIZE);
    std::string input(buffer);

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
      int l = input.find_first_of(LEFT_BRACE);
      int r = input.find_last_of(RIGHT_BRACE);
      std::string args = input.substr(l + 1, r - l - 1);

      std::string arg1 =
          helper::strip(args.substr(0, args.find_first_of(',')), ' ');
      std::string arg2 = args.substr(args.find_first_of(',') + 1, ' ');
      std::vector<helper::Token<T>> tokens =
          helper::expressionToTokens<T>(arg2);
      poly::Polynomial<T> p = calculateExpr(tokens);
      std::cout << "token dumped" << std::endl;

      polynomials.insert(std::pair<std::string, poly::Polynomial<T>>(arg1, p));
    } break;

    case OpType::DISPLAY: {
      printf("Found %d polynomials:\n", polynomials.size());
      for (auto &pair : polynomials) {
        printf("%s(x) = %s\n", pair.first.c_str(),
               pair.second.format().c_str());
      }
    } break;

    case OpType::EVALUATE: {
      int l = input.find_first_of(LEFT_BRACE);
      int r = input.find_last_of(RIGHT_BRACE);
      std::string args = input.substr(l + 1, r - l - 1);

      std::string arg1 =
          helper::strip(args.substr(0, args.find_first_of(',')), ' ');
      std::string arg2 = args.substr(args.find_first_of(',') + 1, ' ');
      if (polynomials.find(arg1) != polynomials.end()) {
        std::cout << polynomials.find(arg1)->second.evaluate(atof(arg2.c_str()))
                  << std::endl;
      } else {
        printf("Can't find polynomials named %s", arg1.c_str());
      }
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
void Application<T>::plot(std::string polyName, double start, double end) {
  if (polynomials.find(polyName) == polynomials.end()) {
    printf(POLYNOMIAL_NOT_FOUND_MESSAGE, polyName);
    std::cout << std::endl;
  }

  std::vector<double> values(canvasExtent.width);
  double paceX = (end - start) / (canvasExtent.width - 1);
  for (int i = 0; i < canvasExtent.width; i++) {
    values.push_back(polynomials[polyName].evaluate(start + paceX * i));
  }

  double baseX = start, baseY = std::min(values);
  double rangeX = end - start, rangeY = std::max(values) - baseY;
  double paceY = rangeY / (canvasExtent.height - 1);

  helper::repeatPrint(VERTICAL_AXIS_NUMBER_WIDTH - 1, ' ');

  std::cout << "y^" << std::endl;

  for (int i = canvasExtent.height; i > -1; i--) {
    // print vertical axis and occasionally scale
    if (i % GRID_INTERIM_Y == 0) {
      std::cout << std::ios::left << std::setw(VERTICAL_AXIS_NUMBER_WIDTH)
                << i * paceY << '|';
    } else {
      helper::repeatPrint(VERTICAL_AXIS_NUMBER_WIDTH, ' ');
      std::cout << '|';
    }

    // print dots. No value: plus; Dots on curve: asterisk
    for (double value : values) {
      double relativeOffset = (value - baseY) / paceY;
      if (relativeOffset >= -0.5 && relativeOffset < 0.5) {
        std::cout << '*';
      } else {
        std::cout << '+';
      }
    }
  }

  // print horizontal axis
  helper::repeatPrint(VERTICAL_AXIS_NUMBER_WIDTH + 1, ' ');
  helper::repeatPrint(canvasExtent.width, '-');
  std::cout << ">x" << std::endl;

  // occasionally print scale
  helper::repeatPrint(VERTICAL_AXIS_NUMBER_WIDTH)
}
} // namespace app
