#include "constants.h"
#include "polynomial.hpp"
#include <cstring>
#include <iostream>
#include <set>
#include <stack>

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

std::string strip(const std::string &original, char ch) {
  std::string t = original;
  while (t.find(ch) != std::string::npos) {
    t.erase(t.find(ch));
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

int skipSpace(const std::string &str, int cur) {
  int i = cur;
  for (; i < str.length(); i++) {
    if (str[i] != ' ') {
      break;
    }
  }
  return i;
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
  return atof(buffer.c_str());
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
  return atoi(buffer.c_str());
}

template <typename T>
poly::Polynomial<T> parsePolynomial(const std::string &expr) {
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
    if (expr[i] != 'x') {
      items.push_back(std::pair<T, uint32_t>(coefficient, 0));
      i++;
    } else {
      i++;
      if (i >= expr.size() || expr[i] != '^') {
        items.push_back(std::pair<T, uint32_t>(coefficient, 1));
      } else {
        i++;
        uint32_t exponent = readInt(expr, i);
        items.push_back(std::pair<T, uint32_t>(coefficient, exponent));
      }
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
  for (int i = 0; i < expr.size(); i++) {
    if (delimiters.find(expr[i]) == delimiters.end()) {
      buffer = buffer + expr[i];
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
      switch (expr[i]) {
      case LEFT_BRACKET: {
        buffer = expr.substr(i + 1);
        buffer = buffer.substr(0, buffer.find_first_of(RIGHT_BRACKET));
        TokenData<T> data;
        data.polynomial = parsePolynomial<T>(buffer);
        Token<T> t;
        t.tp = TokenTypes::POLY;
        t.data = data;
        tokens.push_back(t);
        std::vector<std::pair<T, uint32_t>> debug_info =
            t.data.polynomial.dump();
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
        data.brace = (expr[i] == '(') ? TokenBrace::L : TokenBrace::R;
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
    case TokenTypes::POLY: {
      std::vector<std::pair<T, uint32_t>> debug =
          tokens[i].data.polynomial.dump();
    }
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

} // namespace helper

} // namespace app
