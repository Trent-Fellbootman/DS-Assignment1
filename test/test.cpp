#include "polynomial.hpp"
#include <gtest/gtest.h>
#include <vector>
using namespace std;
using namespace poly;
TEST(POW_TEST, pow) {
  EXPECT_EQ(poly::helper::pow(2, 10), 1024);
  EXPECT_EQ(poly::helper::pow(100, 0), 1);
}
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
