#include "polynomial/polynomial.hpp"
#include <iostream>

using namespace std;
using namespace poly;

int main() {
  std::vector<std::pair<int, int>> items;
  Polynomial<int> p(items);
  p.evaluate(0);
  return 0;
}
