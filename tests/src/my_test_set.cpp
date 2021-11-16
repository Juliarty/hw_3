#include "set.hpp"

#include <gtest/gtest.h>

#include <set>
#include <stdexcept>

TEST(createSet, defaultCreationTest) {
  Set<int> set;
  std::set<int> stdSet;

  for (size_t i = 0; i < 100; i++) {
    set.insert((int)i);
  }

  EXPECT_EQ(true, set.find(50));
  set.erase(50);
  EXPECT_EQ(false, set.find(50));
}
