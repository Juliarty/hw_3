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

TEST(manipulationOperations, insertTest) {
  const int ELEMENTS_NUM = 100000;
  Set<double> setA;

  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    setA.insert((double)i);
  }

  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    EXPECT_EQ(true, setA.contains((double)i));
  }
}

TEST(manipulationOperations, eraseTest) {
  const int ELEMENTS_NUM = 100000;
  Set<double> setA;

  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    setA.insert((double)i);
  }

  for (size_t i = 0; i < ELEMENTS_NUM / 2; ++i) {
    setA.erase((double)i);
    setA.erase(ELEMENTS_NUM - (double)i);
  }

  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    if (i == ELEMENTS_NUM / 2) {
      EXPECT_EQ(true, setA.contains((double)i));
    } else {
      EXPECT_EQ(false, setA.contains((double)i));
    }
  }
}

TEST(setIterator, incrementTest) {
  const int ELEMENTS_NUM = 100000;
  Set<double> setA;

  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    setA.insert((double)i);
  }

  int i = 0;
  for (auto it = setA.begin(); it != setA.end(); ++it) {
    EXPECT_EQ(i++, *it);
  }

  EXPECT_EQ(ELEMENTS_NUM, i);
}

TEST(setIterator, decrementTest) {
  const int ELEMENTS_NUM = 100000;
  Set<double> setA;

  for (size_t i = 0; i < ELEMENTS_NUM; ++i) {
    setA.insert((double)i);
  }

  int i = 5;
  auto it = std::next(setA.begin(), 5);

  for (; it != setA.begin(); --it) {
    EXPECT_EQ(i--, *it);
  }

  EXPECT_EQ(i, *it);
  EXPECT_EQ(0, i);
}