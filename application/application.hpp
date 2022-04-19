#include "constants.h"
#include "helper.hpp"
#include "logger.hpp"
#include "polynomial.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#define BUFFER_SIZE 1024

namespace app {
class variable_not_found_exception : public std::exception {
private:
  std::string err;

public:
public:
  variable_not_found_exception(std::string var) {
    this->err = "Can't find variable " + var;
  }
  const char *what() const noexcept override final { return err.c_str(); }
};
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
  Extent canvasExtent{80, 24};
  Logger logger;
  std::string plotColor = SGR_FG_WHITE;
  bool gridEnabled = false;
  std::string prompt = DEFAULT_PROMPT;

  void mainLoop();
  void plot(std::string polyName, double start, double end);

  std::unique_ptr<poly::Polynomial<T>> evaluateExpression(std::string expr);
  poly::Polynomial<T>
  calculateExpr(const std::vector<helper::Token<T>> &tokens);
};
} // namespace app

namespace app {
template <typename T>
poly::Polynomial<T>
Application<T>::calculateExpr(const std::vector<helper::Token<T>> &tokens) {
  std::stack<poly::Polynomial<T>> ret;
  std::vector<helper::Token<T>> rpn = helper::expression2RPN(tokens);
  for (int i = 0; i < rpn.size(); i++) {
    if (rpn[i].tp == TokenTypes::OP) {
      if (ret.size() < 1) {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_INVALID_OPERATION);
        logger.setLevel(Logger::Level::NORMAL);
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
          ret.push(v1 * v2);
        } break;
        case CalcOps::DIV: {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_CANNOT_DIVIDE);
          logger.setLevel(Logger::Level::NORMAL);
        } break;
        }
      }

    } else if (rpn[i].tp == TokenTypes::VAR) {
      auto it = polynomials.find(rpn[i].data.variable);
      if (it == polynomials.end()) {
        throw variable_not_found_exception(rpn[i].data.variable);
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
    logger.endLine();
    int indent = logger.getIndent();
    logger.setIndent(0);
    logger.printString(prompt);
    std::cin.getline(buffer, BUFFER_SIZE);
    logger.endLine();
    logger.setIndent(indent);
    std::string input(buffer);

    if (std::count(input.begin(), input.end(), LEFT_BRACE) == 0 ||
        std::count(input.begin(), input.end(), RIGHT_BRACE) == 0) {
      logger.setLevel(Logger::Level::ERROR);
      logger.printString(INVALID_COMMAND_MESSAGE);
      logger.setLevel(Logger::Level::NORMAL);
      logger.endLine();
      continue;
    }

    int index = input.find_first_of(LEFT_BRACE);

    OpType operation = helper::stringToOpType(buffer, index);

    switch (operation) {
    case OpType::HELP: {
      logger.println(HELPER_MESSAGE);
      continue;
    } break;

    case OpType::ASSIGN: {
      int l = input.find_first_of(LEFT_BRACE);
      int r = input.find_last_of(RIGHT_BRACE);
      std::string args = input.substr(l + 1, r - l - 1);
      std::string arg1 =
          helper::strip(args.substr(0, args.find_first_of(',')), ' ');
      std::string arg2 = args.substr(args.find_first_of(',') + 1);
      std::vector<helper::Token<T>> tokens =
          helper::expressionToTokens<T>(arg2);
      try {
        poly::Polynomial<T> p = calculateExpr(tokens);
        auto it = polynomials.find(arg1);
        if (it == polynomials.end()) {
          polynomials.insert(
              std::pair<std::string, poly::Polynomial<T>>(arg1, p));
        } else {
          it->second = p;
        }
      } catch (std::exception &e) {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(e.what());
        logger.setLevel(Logger::Level::NORMAL);
        continue;
      }
    } break;

    case OpType::DISPLAY: {
      logger.println("Found " + std::to_string(polynomials.size()) +
                     " polynomial(s):");
      for (auto &pair : polynomials) {
        logger.println(pair.first + "(x) = " + pair.second.format());
      }
    } break;

    case OpType::EVALUATE: {
      size_t argsStart = input.find_first_of(LEFT_BRACE) + 1;
      size_t argsEnd = input.find_last_of(RIGHT_BRACE);
      std::string rawArgs = helper::strip(
          input.substr(argsStart, argsEnd - argsStart), WHITE_SPACE);
      std::vector<std::string> args = helper::separate(rawArgs, CHAR_COMMA);
      if (args.size() < 2) {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_TOO_FEW_ARGUMENTS);
        logger.setLevel(Logger::Level::NORMAL);
        continue;
      } else if (args.size() > 2) {
        logger.setLevel(Logger::Level::WARNING);
        logger.println(MESSAGE_TOO_MANY_ARGUMENTS);
        logger.setLevel(Logger::Level::NORMAL);
      }
      if (polynomials.find(args[0]) != polynomials.end()) {
        logger.println(std::to_string(
            polynomials.find(args[0])->second.evaluate(atof(args[1].c_str()))));
      } else {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_PREFIX_POLY_NOT_FOUND + args[0]);
        logger.setLevel(Logger::Level::NORMAL);
      }
    } break;

    case OpType::PLOT: {
      size_t argsStart = input.find_first_of(LEFT_BRACE) + 1;
      size_t argsEnd = input.find_last_of(RIGHT_BRACE);
      std::string rawArgs = helper::strip(
          input.substr(argsStart, argsEnd - argsStart), WHITE_SPACE);
      std::vector<std::string> args = helper::separate(rawArgs, CHAR_COMMA);

      if (args.size() < 3) {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_TOO_FEW_ARGUMENTS);
        logger.setLevel(Logger::Level::NORMAL);
        continue;
      } else if (args.size() > 3) {
        logger.setLevel(Logger::Level::WARNING);
        logger.println(MESSAGE_TOO_MANY_ARGUMENTS);
        logger.setLevel(Logger::Level::NORMAL);
      }

      if (polynomials.find(args[0]) == polynomials.end()) {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_PREFIX_POLY_NOT_FOUND + args[0]);
        logger.setLevel(Logger::Level::NORMAL);
        continue;
      }

      double arg1, arg2;

      try {
        arg1 = stod(args[1]);
      } catch (std::exception &e) {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_FAILED_TO_PARSE_EXPRESSIONS);
        logger.setLevel(Logger::Level::NORMAL);
        continue;
      }

      try {
        arg2 = stod(args[2]);
      } catch (std::exception &e) {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_FAILED_TO_PARSE_EXPRESSIONS);
        logger.setLevel(Logger::Level::NORMAL);
        continue;
      }

      if (arg1 >= arg2) {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_INVALID_ARGUMENTS);
        logger.setLevel(Logger::Level::NORMAL);
        continue;
      }

      plot(args[0], arg1, arg2);
    } break;
    case OpType::DELETE: {
      size_t argsStart = input.find_first_of(LEFT_BRACE) + 1;
      size_t argsEnd = input.find_last_of(RIGHT_BRACE);
      std::string rawArgs = helper::strip(
          input.substr(argsStart, argsEnd - argsStart), WHITE_SPACE);

      auto it = polynomials.find(rawArgs);
      if (it == polynomials.end()) {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_PREFIX_POLY_NOT_FOUND + rawArgs);
        logger.setLevel(Logger::Level::NORMAL);
      } else {
        polynomials.erase(it);
      }
    } break;
    case OpType::UNKNOWN: {
      logger.setLevel(Logger::Level::ERROR);
      logger.println(UNKNOWN_OP_MESSAGE);
      logger.setLevel(Logger::Level::NORMAL);
    } break;

    case OpType::SET_PROPERTY: {
      size_t argsStart = input.find_first_of(LEFT_BRACE) + 1;
      size_t argsEnd = input.find_last_of(RIGHT_BRACE);
      std::string rawArgs = helper::strip(
          input.substr(argsStart, argsEnd - argsStart), WHITE_SPACE);
      std::vector<std::string> args = helper::separate(rawArgs, CHAR_COMMA);

      if (args[0] == "CANV_W") {
        if (args.size() < 2) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_TOO_FEW_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (args.size() > 2) {
          logger.setLevel(Logger::Level::WARNING);
          logger.println(MESSAGE_TOO_MANY_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
        }

        int arg1;

        try {
          arg1 = stoi(args[1]);
        } catch (std::exception &e) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_FAILED_TO_PARSE_EXPRESSIONS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        }

        if (arg1 < MIN_CANVAS_WIDTH) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_PREFIX_CANVAS_WIDTH_TOO_SMALL +
                         std::to_string(MIN_CANVAS_WIDTH));
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        }

        canvasExtent.width = arg1;
      } else if (args[0] == "CANV_H") {
        if (args.size() < 2) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_TOO_FEW_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (args.size() > 2) {
          logger.setLevel(Logger::Level::WARNING);
          logger.println(MESSAGE_TOO_MANY_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
        }

        int arg1;

        try {
          arg1 = stoi(args[1]);
        } catch (std::exception &e) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_FAILED_TO_PARSE_EXPRESSIONS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        }

        if (arg1 < MIN_CANVAS_HEIGHT) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_PREFIX_CANVAS_HEIGHT_TOO_SMALL +
                         std::to_string(MIN_CANVAS_HEIGHT));
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        }

        canvasExtent.height = arg1;
      } else if (args[0] == "PLOT_COL") {
        if (args.size() < 4) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_TOO_FEW_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (args.size() > 4) {
          logger.setLevel(Logger::Level::WARNING);
          logger.println(MESSAGE_TOO_MANY_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
        }

        std::vector<std::string> colorArgs(args.begin() + 1, args.end());
        Color newColor = helper::strsToCol(colorArgs);
        if (newColor.rgb[0] == -1) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_FAILED_TO_PARSE_EXPRESSIONS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (newColor.rgb[0] == -2) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_INVALID_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        }

        plotColor = "\e[38;2;" + std::to_string(newColor.rgb[0]) + ";" +
                    std::to_string(newColor.rgb[1]) + ";" +
                    std::to_string(newColor.rgb[2]) + "m";
      } else if (args[0] == PROPERTY_OUTPUT_COLOR) {
        if (args.size() < 4) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_TOO_FEW_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (args.size() > 4) {
          logger.setLevel(Logger::Level::WARNING);
          logger.println(MESSAGE_TOO_MANY_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
        }

        std::vector<std::string> colorArgs(args.begin() + 1, args.end());
        Color newColor = helper::strsToCol(colorArgs);
        if (newColor.rgb[0] == -1) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_FAILED_TO_PARSE_EXPRESSIONS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (newColor.rgb[0] == -2) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_INVALID_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        }

        logger.setSGR_output("\e[38;2;" + std::to_string(newColor.rgb[0]) +
                             ";" + std::to_string(newColor.rgb[1]) + ";" +
                             std::to_string(newColor.rgb[2]) + "m");
      } else if (args[0] == PROPERTY_INPUT_COLOR) {
        if (args.size() < 4) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_TOO_FEW_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (args.size() > 4) {
          logger.setLevel(Logger::Level::WARNING);
          logger.println(MESSAGE_TOO_MANY_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
        }

        std::vector<std::string> colorArgs(args.begin() + 1, args.end());
        Color newColor = helper::strsToCol(colorArgs);
        if (newColor.rgb[0] == -1) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_FAILED_TO_PARSE_EXPRESSIONS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (newColor.rgb[0] == -2) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_INVALID_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        }

        logger.setSGR_input("\e[38;2;" + std::to_string(newColor.rgb[0]) + ";" +
                            std::to_string(newColor.rgb[1]) + ";" +
                            std::to_string(newColor.rgb[2]) + "m");
      } else if (args[0] == "GRID") {
        if (args.size() < 2) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_TOO_FEW_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (args.size() > 2) {
          logger.setLevel(Logger::Level::WARNING);
          logger.println(MESSAGE_TOO_MANY_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
        }

        if (args[1] == "ON") {
          gridEnabled = true;
        } else if (args[1] == "OFF") {
          gridEnabled = false;
        } else {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_PREFIX_UNKNOWN_OPTION + args[1]);
          logger.setLevel(Logger::Level::NORMAL);
        }
      } else if (args[0] == PROPERTY_PROMPT) {
        if (args.size() < 2) {
          logger.setLevel(Logger::Level::ERROR);
          logger.println(MESSAGE_TOO_FEW_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
          continue;
        } else if (args.size() > 2) {
          logger.setLevel(Logger::Level::WARNING);
          logger.println(MESSAGE_TOO_MANY_ARGUMENTS);
          logger.setLevel(Logger::Level::NORMAL);
        }

        prompt = args[1] + " ";
      } else {
        logger.setLevel(Logger::Level::ERROR);
        logger.println(MESSAGE_PREFIX_UNKNOWN_PROPERTY + args[0]);
        logger.setLevel(Logger::Level::NORMAL);
      }
    } break;

    case OpType::CLEAR_VARIABLES: {
      polynomials.clear();
    } break;

    case OpType::CLEAR_CONSOLE: {
      logger.clear();
    } break;

    case OpType::EXIT: {
      logger.restore();
      return;
    }
    }
  }
}

template <typename T>
void Application<T>::plot(std::string polyName, double start, double end) {
  // handle exceptions
  if (polynomials.find(polyName) == polynomials.end()) {
    logger.setLevel(Logger::Level::ERROR);
    logger.println(POLYNOMIAL_NOT_FOUND_MESSAGE);
    logger.setLevel(Logger::Level::NORMAL);
  }

  poly::Polynomial<T> &polynomial = polynomials[polyName];

  logger.println("Plotting: " + polyName + "(x) = " + polynomial.format());
  logger.println("From " + std::to_string(start) + " to " +
                 std::to_string(end));

  // calculate values
  std::vector<double> values(canvasExtent.width);
  double paceX = (end - start) / (canvasExtent.width - 1);
  for (int i = 0; i < canvasExtent.width; i++) {
    values[i] = (polynomial.evaluate(start + paceX * i));
  }

  double baseX = start, baseY = *std::min_element(values.begin(), values.end());
  baseY -= EPSILON;
  double rangeX = end - start,
         rangeY = *std::max_element(values.begin(), values.end()) - baseY;
  rangeY += EPSILON;
  double paceY = rangeY / (canvasExtent.height - 1);

  // print y label
  logger.pad(VERTICAL_AXIS_NUMBER_WIDTH - 1);

  logger.printString("y ^");
  logger.endLine();

  for (int i = canvasExtent.height; i > -1; i--) {
    // print vertical axis and occasionally scale
    if (i % GRID_INTERIM_Y == 0) {
      logger.printNumber(baseY + i * paceY, {Logger::Alignment::RIGHT,
                                             VERTICAL_AXIS_NUMBER_WIDTH});
      logger.printString("-|");
    } else {
      logger.pad(VERTICAL_AXIS_NUMBER_WIDTH);
      logger.printString(" |");
    }

    // print dots. No value: plus; Dots on curve: asterisk
    for (double value : values) {
      double relativeOffset = (value - baseY) / paceY - i;
      if (relativeOffset >= -0.5 && relativeOffset < 0.5) {
        logger.putchar('*', plotColor);
      } else {
        if (gridEnabled) {
          logger.putchar('+');
        } else {
          logger.putchar(WHITE_SPACE);
        }
      }
    }
    logger.endLine();
  }

  // print horizontal axis
  logger.pad(VERTICAL_AXIS_NUMBER_WIDTH + 2);
  logger.pad(canvasExtent.width, '-');
  logger.printString("> x");
  logger.endLine();

  // print scale indicators
  logger.pad(VERTICAL_AXIS_NUMBER_WIDTH + 2);
  for (int i = 0; i < canvasExtent.width / GRID_INTERIM_X; i++) {
    logger.pad(GRID_INTERIM_X - 1);
    logger.printString("|");
  }
  logger.endLine();

  // print scale
  logger.pad(VERTICAL_AXIS_NUMBER_WIDTH + 2);
  logger.pad(HORIZONTAL_AXIS_NUMBER_WIDTH / 2);
  for (int i = 0; i < canvasExtent.width / GRID_INTERIM_X; i++) {
    logger.pad(GRID_INTERIM_X - HORIZONTAL_AXIS_NUMBER_WIDTH);
    logger.printNumber(
        baseX + ((i + 1) * GRID_INTERIM_X - 1) * paceX,
        {Logger::Alignment::CENTER, HORIZONTAL_AXIS_NUMBER_WIDTH});
  }
  logger.endLine();
}
} // namespace app
