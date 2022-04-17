#include "polynomial/polynomial.hpp"
#include <iostream>

using namespace std;
using namespace poly;

int main() {
  std::vector<std::pair<int, int>> items1{{1, 1}};
  /*items1.push_back(std::pair<int, int>(1, 10));
  items1.push_back(std::pair<int, int>(2, 0));
  items1.push_back(std::pair<int, int>(78, 3));
  Polynomial<int> p1(items1);
  std::vector<std::pair<int, int>> items2;
  items2.push_back(std::pair<int, int>(2, 1));
  Polynomial<int> p2(items2);
  Polynomial<int> p3 = p1 + p2;
  // p3 = p3 * 2;
  std::unique_ptr<Polynomial<int>> p4 = p3.copy();
  std::cout << p4->evaluate(2);*/
  Polynomial<int> p1(items1);
  std::unique_ptr<Polynomial<int>> p2 = p1.copy();

  return 0;
}
