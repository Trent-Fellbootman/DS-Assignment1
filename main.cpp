#include "polynomial/polynomial.hpp"
#include <iostream>

using namespace std;
using namespace poly;

int main() {
  std::vector<std::pair<int, int>> items;
  items.push_back(std::pair<int, int>(2, 0));
  Polynomial<int> p(items);
  std::cout << p.evaluate(0) << std::endl;
  return 0;
}
