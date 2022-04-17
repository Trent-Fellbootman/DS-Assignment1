#pragma once
#include "polynomial.hpp"
#include <map>
#include <string>

template <typename T>
class Application {

public:
  Application() = default;
  void run();

private:
  std::map<std::string, poly::Polynomial<T>> polynomials;

  void mainLoop();
};