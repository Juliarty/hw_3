#include "set.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <set>
#include <stdexcept>
#include <time.h>
#include <vector>

#define TEST_DATA_ELEMENTS_NUM 1e6
#define TEST_PERFORMANCE_DECREASE_COEFF 3

void speedTestFramework(
    void (*myFunc)(Set<int> &, int), void (*stdFunc)(std::set<int> &, int),
    const size_t elementsNum = TEST_DATA_ELEMENTS_NUM,
    unsigned int decreaseCoef = TEST_PERFORMANCE_DECREASE_COEFF) {
  static const size_t kMaxElement = elementsNum / 3;
  std::vector<int> data(elementsNum);
  EXPECT_EQ(data.size(), elementsNum);
  std::mt19937 gen(42);
  std::for_each(data.begin(), data.end(),
                [&](int &a) { a = gen() % kMaxElement; });
  Set<int> mySet(data.begin(), data.end());
  std::set<int> stdSet(data.begin(), data.end());
  int myStart = clock();
  for (auto it = data.begin(); it != data.end(); ++it) {
    myFunc(mySet, *it);
  }
  int myEnd = clock();

  int stdStart = clock();
  for (auto it = data.begin(); it != data.end(); ++it) {
    stdFunc(stdSet, *it);
  }
  int stdEnd = clock();

  EXPECT_LE((myEnd - myStart), decreaseCoef * (stdEnd - stdStart));
}

TEST(speedTest, creationSpeedTest) {
  speedTestFramework([](Set<int> &set, int value) { set.insert(value); },
                     [](std::set<int> &set, int value) { set.insert(value); });
}

TEST(speedTest, eraseSpeedTest) {
  speedTestFramework([](Set<int> &set, int value) { set.erase(value); },
                     [](std::set<int> &set, int value) { set.erase(value); });
}

TEST(speedTest, findSpeedTest) {
  speedTestFramework([](Set<int> &set, int value) { set.find(value); },
                     [](std::set<int> &set, int value) { set.find(value); });
}

TEST(speedTest, iteratorSpeedTest) {
  static const size_t kElementsNum = 1e6;
  static const size_t kIteratorShift = 100;
  speedTestFramework(
      [](Set<int> &set, int value) { std::next(set.begin(), kIteratorShift); },
      [](std::set<int> &set, int value) {
        std::next(set.begin(), kIteratorShift);
      },
      kElementsNum);
}