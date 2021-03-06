#include "set.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <set>
#include <stdexcept>
#include <time.h>
#include <vector>

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
  static const int kElementsNum = 100;
  Set<double> setA;

  for (size_t i = 0; i < kElementsNum; ++i) {
    setA.insert((double)i);
  }

  Set<double> setB(setA);

  for (size_t i = 0; i < kElementsNum; ++i) {
    setA.erase((double)i);
    EXPECT_EQ(true, setB.contains((double)i));
  }
}

TEST(createSet, assigmentOpetorTest) {
  static const int kElementsNum = 100;
  Set<double> setA, setB;

  for (size_t i = 0; i < kElementsNum; ++i) {
    setA.insert((double)i);
    setB.insert((double)-i);
  }

  setA = setB;
  for (size_t i = 0; i < kElementsNum; ++i) {
    setB.erase((double)i);
    EXPECT_EQ(true, setA.contains((double)-i));
  }
}

TEST(manipulationOperations, insertTest) {
  static const int kElementsNum = 100000;
  Set<double> setA;

  for (size_t i = 0; i < kElementsNum; ++i) {
    setA.insert((double)i);
  }

  for (size_t i = 0; i < kElementsNum; ++i) {
    EXPECT_EQ(true, setA.contains((double)i));
  }
}

TEST(manipulationOperations, eraseTest) {
  static const int kElementsNum = 100000;
  Set<double> setA;

  for (size_t i = 0; i < kElementsNum; ++i) {
    setA.insert((double)i);
  }

  for (size_t i = 0; i < kElementsNum / 2; ++i) {
    setA.erase((double)i);
    setA.erase(kElementsNum - (double)i);
  }

  for (size_t i = 0; i < kElementsNum; ++i) {
    if (i == kElementsNum / 2) {
      EXPECT_EQ(true, setA.contains((double)i));
    } else {
      EXPECT_EQ(false, setA.contains((double)i));
    }
  }
}

TEST(setIterator, incrementTest) {
  static const size_t kElementsNum = 100000;
  Set<double> set;

  for (size_t i = 0; i < kElementsNum; ++i) {
    set.insert((double)i);
  }

  int i = 0;
  for (auto it = set.begin(); it != set.end(); ++it) {
    EXPECT_EQ(i++, *it);
  }

  EXPECT_EQ(kElementsNum, i);
}

TEST(setIterator, decrementTest) {
  static const size_t kElementsNum = 100000;
  Set<double> set;

  for (size_t i = 0; i < kElementsNum; ++i) {
    set.insert((double)i);
  }

  int i = kElementsNum;
  auto it = --set.end();
  for (; it != set.begin(); --it) {
    EXPECT_EQ(--i, *it);
  }

  EXPECT_EQ(0, *it);
}

TEST(setIterator, dereferenceTest) {
  Set<int> set{-1, 0, 1};
  auto it = set.begin();
  EXPECT_EQ(-1, *it);
}

TEST(setIterator, postIncrementTest) {
  Set<int> set{-1, 0, 1};
  auto it = set.begin();
  EXPECT_EQ(-1, *it++);
}

TEST(setIterator, postDecrementTest) {
  Set<int> set{-1, 0, 1};
  auto it = --set.end();
  EXPECT_EQ(1, *it--);
}

TEST(setIterator, rangeForTest) {
  Set<int> set{0, 1, 2, 3, 4};
  int i = 0;
  for (auto el : set) {
    EXPECT_EQ(i++, el);
  }
  EXPECT_EQ(5, i);
}

TEST(constMethods, sizeTest) {
  static const size_t kElementsNum = 123456;
  Set<int> set;

  for (size_t i = 0; i < kElementsNum; ++i) {
    set.insert(i * 5 + 1);
  }

  EXPECT_EQ(kElementsNum, set.size());
  set.clear();
  EXPECT_EQ(0, set.size());

  set.insert(1);
  EXPECT_NE(0, set.size());

  set.erase(1);
  EXPECT_EQ(0, set.size());
}

TEST(constMethods, emptyTest) {
  Set<int> set;
  EXPECT_EQ(true, set.empty());

  set.insert(1);
  EXPECT_EQ(false, set.empty());

  set.erase(1);
  EXPECT_EQ(true, set.empty());
}

TEST(constMethods, findTest) {
  Set<int> set{-1, 0, 1, 2, 4, 5};

  EXPECT_EQ(1, *set.find(1));
  EXPECT_EQ(5, *set.find(5));

  set.erase(1);
  EXPECT_EQ(set.end(), set.find(1));
}

TEST(constMethods, lowerBoundTest) {
  Set<int> set{-1, 0, 1, 2, 4, 5};
  EXPECT_EQ(-1, *set.lower_bound(-2));
  EXPECT_EQ(-1, *set.lower_bound(-1));
  EXPECT_EQ(4, *set.lower_bound(3));
  EXPECT_EQ(set.end(), set.lower_bound(6));
}

TEST(integrationTest, randomElementsTest) {
  static const size_t kMaxElement = 1e5;
  static const size_t kElementsNum = 1e5;
  std::array<int, kElementsNum> data;
  std::mt19937 gen(42);
  std::for_each(data.begin(), data.end(),
                [&](int &a) { a = gen() % kMaxElement; });

  Set<int> mySet(data.begin(), data.end());
  std::set<int> stdSet(data.begin(), data.end());

  std::set<int>::iterator stdIt;
  Set<int>::iterator myIt;

  for (myIt = mySet.begin(), stdIt = stdSet.begin(); myIt != --mySet.end();) {
    auto el = *stdIt;
    EXPECT_EQ(*(stdIt++), *(myIt++));
    EXPECT_EQ(*(stdIt--), *(myIt--));
    EXPECT_EQ(*(++stdIt), *(++myIt));
    EXPECT_EQ(*(--stdIt), *(--myIt));
    EXPECT_EQ(*(++stdIt), *(++myIt));

    EXPECT_EQ(stdSet.size(), mySet.size());
    EXPECT_EQ(*stdSet.find(el), *mySet.find(el));
    mySet.erase(el);
    stdSet.erase(el);
    EXPECT_EQ(stdSet.end(), stdSet.find(el));
    EXPECT_EQ(mySet.end(), mySet.find(el));
    EXPECT_EQ(stdSet.size(), mySet.size());

    mySet.insert(el);
    stdSet.insert(el);
    EXPECT_EQ(stdSet.size(), mySet.size());
    EXPECT_EQ(*stdSet.find(el), *mySet.find(el));

    mySet.erase(el);
    stdSet.erase(el);

    auto tmp = gen() % kMaxElement;
    auto myLowerBound = mySet.lower_bound(tmp);
    auto stdLowerBound = stdSet.lower_bound(tmp);

    if (stdLowerBound == stdSet.end()) {
      EXPECT_EQ(mySet.end(), myLowerBound);
    } else {
      EXPECT_EQ(*stdLowerBound, *myLowerBound);
    }
  }
  stdSet.clear();
  mySet.clear();
  EXPECT_EQ(stdSet.size(), mySet.size());
  EXPECT_EQ(stdSet.empty(), mySet.empty());
}
