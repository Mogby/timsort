#pragma once

#ifndef TEST_SEQUENCE
#define TEST_SEQUENCE

#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "timsort.h"
#include "test_generator.h"

template <class RandomAccessIterator>
bool areRangesEqual(RandomAccessIterator firstBegin, RandomAccessIterator firstEnd,
        RandomAccessIterator secondBegin, RandomAccessIterator secondEnd) {
    if (firstEnd - firstBegin != secondEnd - secondBegin) {
        return false;
    }

    while (firstBegin != firstEnd) {
        if (*firstBegin != *secondBegin) {
            return false;
        }
        ++firstBegin;
        ++secondBegin;
    }

    return true;
}

struct TestResult {
    float timSortTime;
    float stdSortTime;
};

void printTestMessage(bool success, ui32 testSize, ECollisionProbability collisionProbability,
        TestResult sortTimes) {
    std::string collisionProbabilityString = "";
    switch (collisionProbability) {
        case CP_LOW:
            collisionProbabilityString = "low";
            break;
        case CP_MEDIUM:
            collisionProbabilityString = "medium";
            break;
        case CP_HIGH:
            collisionProbabilityString = "high";
            break;
    }

    std::cout << (success ? "PASSED" : "FAILED") << " TEST:" << " test; size: " << testSize << 
        "; collision probability: " << collisionProbabilityString << std::endl;
    std::cout << "\ttimsort time:\t" << std::setprecision(4) << sortTimes.timSortTime << std::endl;
    std::cout << "\tstd::sort time:\t" << std::setprecision(4) << sortTimes.stdSortTime << std::endl;
    std::cout << std::endl;
}

template <class RandomAccessIterator, class Compare>
TestResult runSorts(RandomAccessIterator firstBegin, RandomAccessIterator firstEnd,
        RandomAccessIterator secondBegin, RandomAccessIterator secondEnd,
        Compare comp) {
    TestResult result;

    clock_t testClock = clock();
    timSort(firstBegin, firstEnd, comp);
    testClock = clock() - testClock;

    result.timSortTime = static_cast<float>(testClock) / CLOCKS_PER_SEC;

    testClock = clock();
    std::sort(secondBegin, secondEnd, comp);
    testClock = clock() - testClock;

    result.stdSortTime = static_cast<float>(testClock) / CLOCKS_PER_SEC;

    return result;
}

template <class DataType, class Compare = LessCompare<DataType>>
bool runVectorTest(ui32 testSize, ECollisionProbability collisionProbability, 
        TestGenerator &generator, Compare comp = Compare()) {
    std::vector<DataType> testVector, controlVector; 
    testVector = generator.generateVectorTest<DataType>(testSize, collisionProbability);
    controlVector = testVector;

    TestResult sortTimes = runSorts(testVector.begin(), testVector.end(),
        controlVector.begin(), controlVector.end(), comp);

    bool result = areRangesEqual(testVector.begin(), testVector.end(), controlVector.begin(), controlVector.end());
    printTestMessage(result, testSize, collisionProbability, sortTimes);

    return result;
}

template <class DataType, class Compare = LessCompare<DataType>>
bool runArrayTest(ui32 testSize, ECollisionProbability collisionProbability, TestGenerator &generator,
        Compare comp = Compare()) {
    DataType *testArray = new DataType[testSize];
    DataType *controlArray = new DataType[testSize];
    testArray = generator.generateArrayTest<DataType>(testSize, collisionProbability);
    for (ui32 pointer = 0; pointer < testSize; ++pointer) {
        controlArray[pointer] = testArray[pointer];
    }

    TestResult sortTimes = runSorts(testArray, testArray + testSize,
        controlArray, controlArray + testSize, comp);
    bool result = areRangesEqual(testArray, testArray + testSize, controlArray, controlArray + testSize);

    printTestMessage(result, testSize, collisionProbability, sortTimes);

    delete[] testArray;
    delete[] controlArray;

    return result;
}

void runTestSequence(ui32 testsCount = 1, ui32 maxTestSize = 110000) {
    srand(0451);

    TestGenerator generator;

    std::vector<ui32> testSizes(testsCount);
    while (testsCount--) {
        testSizes[testsCount] = rand() % maxTestSize;
    }

    std::cout << "vector<int> tests:" << std::endl;
    for (auto it = testSizes.begin(); it != testSizes.end(); ++it) {
        runVectorTest<int>(*it, CP_LOW, generator);
        runVectorTest<int>(*it, CP_MEDIUM, generator);
        runVectorTest<int>(*it, CP_HIGH, generator);
    }

    std::cout << "array of int tests:" << std::endl;
    for (auto it = testSizes.begin(); it != testSizes.end(); ++it) {
        runArrayTest<int>(*it, CP_LOW, generator);
        runArrayTest<int>(*it, CP_MEDIUM, generator);
        runArrayTest<int>(*it, CP_HIGH, generator);
    }

    std::greater<int> comp;
    std::cout << "int ascending sort tests:" << std::endl;
    for (auto it = testSizes.begin(); it != testSizes.end(); ++it) {
        runArrayTest<int>(*it, CP_LOW, generator, comp);
        runArrayTest<int>(*it, CP_MEDIUM, generator, comp);
        runArrayTest<int>(*it, CP_HIGH, generator, comp);
    }

    std::cout << "vector<float> tests:" << std::endl;
    for (auto it = testSizes.begin(); it != testSizes.end(); ++it) {
        runVectorTest<float>(*it, CP_LOW, generator);
        runVectorTest<float>(*it, CP_MEDIUM, generator);
        runVectorTest<float>(*it, CP_HIGH, generator);
    }

    std::cout << "array of pair<int, int> tests:" << std::endl;
    for (auto it = testSizes.begin(); it != testSizes.end(); ++it) {
        runArrayTest<std::pair<int, int>>(*it, CP_LOW, generator);
        runArrayTest<std::pair<int, int>>(*it, CP_MEDIUM, generator);
        runArrayTest<std::pair<int, int>>(*it, CP_HIGH, generator);
    }

    std::cout << "array of string test:" << std::endl;
    for (auto it = testSizes.begin(); it != testSizes.end(); ++it) {
        runArrayTest<std::string>(*it, CP_LOW, generator);
        runArrayTest<std::string>(*it, CP_MEDIUM, generator);
        runArrayTest<std::string>(*it, CP_HIGH, generator);
   }

}

#endif
