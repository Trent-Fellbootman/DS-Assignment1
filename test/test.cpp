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
  std::vector<std::pair<int, uint32_t>> items;
  items.push_back(std::pair<int, uint32_t>(1, 10));
  items.push_back(std::pair<int, uint32_t>(2, 0));
  items.push_back(std::pair<int, uint32_t>(78, 3));
  Polynomial<int> p(items);
  EXPECT_EQ(p.evaluate(2), 1650);
}

TEST(EVALUATE_TEST_2, evaluate) {
  std::vector<std::pair<int, uint32_t>> items;
  items.push_back(std::pair<int, uint32_t>(1, 10));
  items.push_back(std::pair<int, uint32_t>(78, 3));
  Polynomial<int> p(items);
  EXPECT_EQ(p.evaluate(2), 1648);
}

TEST(POLYNOMIAL_ADD_1, add) {
  std::vector<std::pair<int, uint32_t>> items1;
  items1.push_back(std::pair<int, uint32_t>(1, 10));
  items1.push_back(std::pair<int, uint32_t>(2, 0));
  items1.push_back(std::pair<int, uint32_t>(78, 3));
  Polynomial<int> p1(items1);
  std::vector<std::pair<int, uint32_t>> items2;
  items2.push_back(std::pair<int, uint32_t>(2, 1));
  Polynomial<int> p2(items2);
  Polynomial<int> p3 = p1 + p2;
  EXPECT_EQ(p3.evaluate(3), 61163);
}

TEST(POLYNOMIAL_ADD_2, add) {
  std::vector<std::pair<int, uint32_t>> items1;
  items1.push_back(std::pair<int, uint32_t>(1, 10));
  items1.push_back(std::pair<int, uint32_t>(2, 0));
  items1.push_back(std::pair<int, uint32_t>(78, 3));
  Polynomial<int> p1(items1);
  std::vector<std::pair<int, uint32_t>> items2;
  items2.push_back(std::pair<int, uint32_t>(2, 3));
  Polynomial<int> p2(items2);
  Polynomial<int> p3 = p1 + p2;
  EXPECT_EQ(p3.evaluate(3), 61211);
}

TEST(POLYNOMIAL_ADD_3, add) {
  std::vector<std::pair<int, uint32_t>> items1;
  items1.push_back(std::pair<int, uint32_t>(1, 10));
  items1.push_back(std::pair<int, uint32_t>(2, 0));
  items1.push_back(std::pair<int, uint32_t>(78, 3));
  Polynomial<int> p1(items1);
  std::vector<std::pair<int, uint32_t>> items2;
  items2.push_back(std::pair<int, uint32_t>(2, 3));
  items2.push_back(std::pair<int, uint32_t>(-1, 10));
  Polynomial<int> p2(items2);
  Polynomial<int> p3 = p1 + p2;
  EXPECT_EQ(p3.evaluate(3), 2162);
}

TEST(POLYNOMIAL_MULTIPLY, multiply) {
  std::vector<std::pair<int, uint32_t>> items1;
  items1.push_back(std::pair<int, uint32_t>(1, 5));
  items1.push_back(std::pair<int, uint32_t>(2, 0));
  items1.push_back(std::pair<int, uint32_t>(78, 3));
  Polynomial<int> p1(items1);

  for (int j = 2; j < 5; j++) {
    int base = p1.evaluate(j);
    for (int i = 0; i < 10; i++) {
      Polynomial<int> ret = p1 * i;
      EXPECT_EQ(ret.evaluate(j), base * i);
    }
  }
}

TEST(POLYNOMIAL_DIVISION, division) {
  std::vector<std::pair<int, uint32_t>> items1;
  items1.push_back(std::pair<int, uint32_t>(56, 5));
  items1.push_back(std::pair<int, uint32_t>(2, 0));
  items1.push_back(std::pair<int, uint32_t>(78, 3));
  Polynomial<int> p1(items1);
  Polynomial<int> p2 = p1 / 2;
  EXPECT_EQ(p1.evaluate(100) / 2, p2.evaluate(100));
}

TEST(COPY, copy) {
  std::vector<std::pair<int, uint32_t>> items1;
  items1.push_back(std::pair<int, uint32_t>(56, 5));
  items1.push_back(std::pair<int, uint32_t>(2, 0));
  items1.push_back(std::pair<int, uint32_t>(78, 3));
  Polynomial<int> p1(items1);
  Polynomial<int> p2 = p1;
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(p1.evaluate(i), p2.evaluate(i));
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
