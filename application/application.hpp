#include "constants.h"
#include "helper.hpp"
#include "polynomial.hpp"
#include <iostream>
#include <map>
#include <memory>
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
  Extent canvasExtent{80, 24};
};
} // namespace app

namespace app {
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
      // todo:calculation
      std::cout << "token dumped" << std::endl;
      /*polynomials.insert(std::pair<std::string, poly::Polynomial<T>>(
          arg1, helper::parsePolynomial<T>(arg2)));*/
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
  std::vector<double> values(canvasExtent.width);
  double pace = canvasExtent.width / (canvasExtent.width - 1);
}
} // namespace app
