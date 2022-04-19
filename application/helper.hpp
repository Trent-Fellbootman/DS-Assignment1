#include "constants.h"
#include "exceptions.hpp"
#include "polynomial.hpp"
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

namespace app {

namespace helper {

template <typename T> struct TokenData {
  std::string variable;
  poly::Polynomial<T> polynomial;
  CalcOps operation;
  TokenBrace brace;
};

template <typename T> struct Token {
  TokenTypes tp;
  TokenData<T> data;
};

OpType stringToOpType(char *buffer, int length) {
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
  } else if (strcmp(buffer, OP_DELETE) == 0) {
    res = OpType::DELETE;
  } else if (strcmp(buffer, OP_HELP) == 0) {
    res = OpType::HELP;
  } else if (strcmp(buffer, OP_EXIT) == 0) {
    res = OpType::EXIT;
  } else if (strcmp(buffer, OP_SET_PROPERTY) == 0) {
    res = OpType::SET_PROPERTY;
  } else if (strcmp(buffer, OP_CLEAR_VARIABLES) == 0) {
    res = OpType::CLEAR_VARIABLES;
  } else if (strcmp(buffer, OP_CLEAR_CONSOLE) == 0) {
    res = OpType::CLEAR_CONSOLE;
  } else {
    res = OpType::UNKNOWN;
  }
  buffer[length] = tmp;
  return res;
}

std::string strip(const std::string &original, char ch) {
  std::string t = "";
  for (int i = 0; i < original.size(); i++) {
    if (original[i] != ch) {
      t = t + original[i];
    }
  }
  return t;
}

bool isNum(char c) { return (c >= '0' && c <= '9') || c == '.'; }

int getP(CalcOps op) {
  switch (op) {
  case CalcOps::ADD:
  case CalcOps::SUB:
    return 1;
  case CalcOps::MUL:
  case CalcOps::DIV:
    return 2;
  }
  return -1;
}

double readDouble(const std::string &str, int &p) {
  std::string buffer = "";
  for (; p < str.length(); p++) {
    if (!isNum(str[p])) {
      break;
    } else {
      buffer = buffer + str[p];
    }
  }
  return std::stod(buffer.c_str());
}

int readInt(const std::string &str, int &p) {
  std::string buffer = "";
  for (; p < str.length(); p++) {
    if (!isNum(str[p])) {
      break;
    } else {
      buffer = buffer + str[p];
    }
  }
  return std::stoi(buffer.c_str());
}

template <typename T>
poly::Polynomial<T> parsePolynomial(const std::string &expr) {
  std::set<uint32_t> exps;
  std::vector<std::pair<T, uint32_t>> items;
  bool neg;
  for (int i = 0; i < expr.size();) {
    if (expr[i] == '+') {
      i++;
    }
    if (expr[i] == '-') {
      neg = true;
      i++;
    } else {
      neg = false;
    }
    T coefficient;
    if (isNum(expr[i])) {
      coefficient = readDouble(expr, i);
    } else {
      coefficient = 1;
    }
    if (neg) {
      coefficient = -coefficient;
    }
    if (i == expr.size()) {
      if (exps.find(0) != exps.end()) {
        throw invalid_polynomial_exception();
      }
      items.push_back(std::pair<T, uint32_t>(coefficient, 0));
      exps.insert(0);
      break;
    } else if (expr[i] != 'x' && (expr[i] == '+' || expr[i] == '-')) {
      if (exps.find(0) != exps.end()) {
        throw invalid_polynomial_exception();
      }
      exps.insert(0);

      items.push_back(std::pair<T, uint32_t>(coefficient, 0));
      i++;
    } else if (expr[i] == 'x') {
      i++;
      if (i >= expr.size()) {
        if (exps.find(1) != exps.end()) {
          throw invalid_polynomial_exception();
        }

        items.push_back(std::pair<T, uint32_t>(coefficient, 1));
        exps.insert(1);
        break;
      }
      if (expr[i] != '^') {
        if (exps.find(1) != exps.end()) {
          throw invalid_polynomial_exception();
        }

        items.push_back(std::pair<T, uint32_t>(coefficient, 1));
        exps.insert(1);
      } else {
        i++;
        uint32_t exponent = readInt(expr, i);
        if (exps.find(exponent) != exps.end()) {
          throw invalid_polynomial_exception();
        }

        items.push_back(std::pair<T, uint32_t>(coefficient, exponent));
        exps.insert(exponent);
      }
    } else {
      throw invalid_polynomial_exception();
    }
  }

  return poly::Polynomial<T>(items);
}

template <typename T>
std::vector<Token<T>> expressionToTokens(const std::string &expr) {
  std::string stripped = strip(expr, WHITE_SPACE);
  std::set<char> delimiters{LEFT_BRACE,    RIGHT_BRACE, LEFT_BRACKET,
                            RIGHT_BRACKET, PLUS_CH,     MINUS_CH,
                            MULTIPLY_CH,   DIVIDE_CH};
  std::vector<Token<T>> tokens;
  std::string buffer = "";
  TokenTypes ct;
  for (int i = 0; i < stripped.size(); i++) {
    if (delimiters.find(stripped[i]) == delimiters.end()) {
      buffer = buffer + stripped[i];
    } else {
      if (!buffer.empty()) {
        TokenData<T> data;
        data.variable = buffer;
        Token<T> t;
        t.tp = TokenTypes::VAR;
        t.data = data;
        tokens.push_back(t);
        buffer = "";
      }
      switch (stripped[i]) {
      case LEFT_BRACKET: {
        buffer = stripped.substr(i + 1);
        buffer = buffer.substr(0, buffer.find_first_of(RIGHT_BRACKET));
        TokenData<T> data;
        data.polynomial = parsePolynomial<T>(buffer);
        Token<T> t;
        t.tp = TokenTypes::POLY;
        t.data = data;
        tokens.push_back(t);
        i = i + buffer.length() + 1;
        buffer = "";
      } break;
      case PLUS_CH: {
        TokenData<T> data;
        data.operation = CalcOps::ADD;
        Token<T> t;
        t.tp = TokenTypes::OP;
        t.data = data;
        tokens.push_back(t);
      } break;
      case MINUS_CH: {
        TokenData<T> data;
        data.operation = CalcOps::SUB;
        Token<T> t;
        t.tp = TokenTypes::OP;
        t.data = data;
        tokens.push_back(t);
      } break;

      case MULTIPLY_CH: {
        TokenData<T> data;
        data.operation = CalcOps::MUL;
        Token<T> t;
        t.tp = TokenTypes::OP;
        t.data = data;
        tokens.push_back(t);
      } break;

      case DIVIDE_CH: {
        TokenData<T> data;
        data.operation = CalcOps::DIV;
        Token<T> t;
        t.tp = TokenTypes::OP;
        t.data = data;
        tokens.push_back(t);
      } break;
      case LEFT_BRACE:
      case RIGHT_BRACE: {
        TokenData<T> data;
        data.brace = (stripped[i] == '(') ? TokenBrace::L : TokenBrace::R;
        Token<T> t;
        t.tp = TokenTypes::BRACE;
        t.data = data;
        tokens.push_back(t);
      } break;
      }
    }
  }
  if (!buffer.empty()) {
    TokenData<T> data;
    data.variable = buffer;
    Token<T> t;
    t.tp = TokenTypes::VAR;
    t.data = data;
    tokens.push_back(t);
    buffer = "";
  }

  return tokens;
} // namespace helper

template <typename T>
std::vector<helper::Token<T>>
expression2RPN(const std::vector<helper::Token<T>> &tokens) {
  std::stack<helper::Token<T>> ts_s;
  std::vector<helper::Token<T>> rpn;
  for (int i = 0; i < tokens.size(); i++) {
    switch (tokens[i].tp) {
    case TokenTypes::BRACE: {
      if (tokens[i].data.brace == TokenBrace::R) {
        bool flag = false;
        while (!ts_s.empty()) {
          Token<T> &token = ts_s.top();
          if (token.tp == TokenTypes::BRACE) {
            flag = true;
            ts_s.pop();
            break;
          }
          rpn.push_back(ts_s.top());
          ts_s.pop();
        }
        if (!flag) {
          printf("Invalid expression ( brace inconsistent)");
        }
      } else {
        ts_s.push(tokens[i]);
      }
    } break;
    case TokenTypes::POLY:
    case TokenTypes::VAR: {
      rpn.push_back(tokens[i]);
      break;
    }
    case TokenTypes::OP: {
      int cp = getP(tokens[i].data.operation);
      while (!ts_s.empty()) {
        Token<T> &token = ts_s.top();
        if (token.tp == TokenTypes::OP) {
          if (getP(token.data.operation) < cp) {
            break;
          }
          rpn.push_back(ts_s.top());
          ts_s.pop();
        } else {
          break;
        }
      }
      ts_s.push(tokens[i]);
    } break;
    }
  }
  while (!ts_s.empty()) {
    rpn.push_back(ts_s.top());
    ts_s.pop();
  }
  return rpn;
}

void repeatPrint(int count, char ch) {
  for (int i = 0; i < count; i++) {
    std::cout << ch;
  }
}

std::vector<std::string> separate(std::string str, char delimiter) {
  if (str.find(delimiter) == std::string::npos) {
    if (str.length() > 0) {
      std::vector<std::string> ret{str};
      return std::move(ret);
    } else {
      std::vector<std::string> ret(0);
      return std::move(ret);
    }
  }

  std::vector<std::string> res;
  size_t lastIndex = -1;
  while (true) {
    size_t newIndex = str.find_first_of(delimiter, lastIndex + 1);
    if (newIndex != std::string::npos) {
      if (newIndex - lastIndex > 1) {
        res.push_back(str.substr(lastIndex + 1, newIndex - lastIndex - 1));
      }
      lastIndex = newIndex;
    } else {
      break;
    }
  }

  if (lastIndex < str.size() - 1) {
    res.push_back(str.substr(lastIndex + 1, str.size() - lastIndex - 1));
  }

  return std::move(res);
}

app::Color strsToCol(std::vector<std::string> &strs) {
  if (strs.size() < 3) {
    return Color{-1, -1, -1};
  }

  app::Color color{-1, -1, -1};

  for (int i = 0; i < 3; i++) {
    try {
      color.rgb[i] = std::stoi(strs[i]);
      if (color.rgb[i] < 0 || color.rgb[i] > 255) {
        return Color{-2, -2, -2};
      }
    } catch (std::exception &e) {
      return Color{-1, -1, -1};
    }
  }
  return color;
}

} // namespace helper

} // namespace app
