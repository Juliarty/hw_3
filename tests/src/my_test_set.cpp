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

  EXPECT_EQ(true, set.contains(50));
  set.erase(50);
  EXPECT_EQ(false, set.contains(50));
}

TEST(createSet, initListCreationTest) {
  Set<double> set({1, 2, 3, 4, 5, 8});
  EXPECT_EQ(true, set.contains(3));
}

TEST(createSet, copyConstructorCreationTest) {
  const int ELEMENTS_NUM = 100;
  Set<double> setA;

  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    setA.insert((double)i);
  }

  Set<double> setB(setA);

  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    setA.erase((double)i);
    EXPECT_EQ(true, setB.contains((double)i));
  }
}

TEST(createSet, assigmentOpetorTest) {
  const int ELEMENTS_NUM = 100;
  Set<double> setA, setB;

  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    setA.insert((double)i);
    setB.insert((double)-i);
  }

  setA = setB;
  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    setB.erase((double)i);
    EXPECT_EQ(true, setA.contains((double)-i));
  }
}