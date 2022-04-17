#include "polynomial.hpp"
#include <gtest/gtest.h>
#include <vector>
using namespace std;
using namespace poly;
TEST(MyTest, add) {
  vector<int> a = {1, 2, 3, 4, 5};
  vector<int> b = {5, 6, 7, 8, 9};
  int current = 6;
  for (int i = 0; i < a.size(); i++) {
    EXPECT_EQ(add(a[i], b[i]), current);
    current += 2;
  }
}
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
