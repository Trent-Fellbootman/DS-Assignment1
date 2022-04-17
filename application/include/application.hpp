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
  struct Extent {
    int width;
    int height;
  };

  std::map<std::string, poly::Polynomial<T>> polynomials;

  void mainLoop();
  void plot(std::string polyName, double start, double end);

  std::unique_ptr<poly::Polynomial<T>> evaluateExpression(std::string expr);
  Extent canvasExtent{80, 24};
};
}
