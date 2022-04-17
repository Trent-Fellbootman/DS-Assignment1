#pragma once
#include "polynomial.hpp"
#include <map>
#include <string>

namespace app {
template <typename T>
class Application {

public:
  Application() = default;
  void run();

private:
  std::map<std::string, poly::Polynomial<T>> polynomials;

  void mainLoop();

  std::unique_ptr<poly::Polynomial<T>> evaluateExpression(std::string expr);
};
}
