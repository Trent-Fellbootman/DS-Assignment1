#include "polynomial.hpp"
#include <gtest/gtest.h>
#include <vector>
using namespace std;
using namespace poly;
TEST(POW_TEST, pow) {
  EXPECT_EQ(poly::helper::pow(2, 10), 1024);
  EXPECT_EQ(poly::helper::pow(100, 0), 1);
}

TEST(EVALUATE_TEST_1, evaluate) {
  std::vector<std::pair<int, int>> items;
  items.push_back(std::pair<int, int>(1, 10));
  items.push_back(std::pair<int, int>(2, 0));
  items.push_back(std::pair<int, int>(78, 3));
  Polynomial<int> p(items);
  EXPECT_EQ(p.evaluate(2), 1650);
}

TEST(EVALUATE_TEST_2, evaluate) {
  std::vector<std::pair<int, int>> items;
  items.push_back(std::pair<int, int>(1, 10));
  items.push_back(std::pair<int, int>(78, 3));
  Polynomial<int> p(items);
  EXPECT_EQ(p.evaluate(2), 1648);
}
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
