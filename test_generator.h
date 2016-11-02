#pragma once

#ifndef TEST_GENERATOR_H
#define TEST_GENERATOR_H

#include <cmath>
#include <vector>

template <class DataType>
class RandomFactory {
public:

    DataType generateObject(ui32 range) {
        return DataType();
    }

};

template <>
class RandomFactory<int> {
public:

    int generateObject(ui32 range) {
        return rand() % range;
    }

};

template <>
class RandomFactory<float> {
public:

    float generateObject(ui32 range) {
        return static_cast<float>(rand()) / RAND_MAX;
    }

};

template <>
class RandomFactory<std::pair<int, int>> {
public:

    std::pair<int, int> generateObject(ui32 range) {
        return std::pair<int, int> (rand() % static_cast<int>(sqrt(range)), 
                rand() % static_cast<int>(sqrt(range)));
    }

};

template <>
class RandomFactory<std::string> {
public:

    std::string generateObject(ui32 range) {
        std::string result = "";
        while (range) {
            result += 'a' + range % 26;
            range /= 26;
        }
        return result;
    }

};

enum ECollisionProbability {
    CP_LOW,
    CP_MEDIUM,
    CP_HIGH
};

class TestGenerator {
private:

    template <class RandomAccessIterator, class DataType>
    void generateRange(RandomAccessIterator begin, RandomAccessIterator end, 
            RandomFactory<DataType> factory, ui32 valueRange) {
        while (begin != end) {
            *begin = factory.generateObject(valueRange);
            ++begin;
        }
    }

    ui32 getValueRange(ui32 testSize, ECollisionProbability collisionProbability) {
        switch (collisionProbability) {
            case CP_LOW:
                if (testSize > 30000) {
                    return testSize;
                }
                return testSize * testSize;
            case CP_MEDIUM:
                return testSize;
            case CP_HIGH:
                return static_cast<ui32>(sqrt(testSize));
        }
    }

public:

    template <class DataType>
    std::vector<DataType> generateVectorTest(ui32 testSize, ECollisionProbability collisionProbability) {
        ui32 valueRange = getValueRange(testSize, collisionProbability);
        
        std::vector<DataType> result(testSize);
        generateRange(result.begin(), result.end(), RandomFactory<DataType>(), valueRange);
        return result;
    }

    template <class DataType>
    DataType* generateArrayTest(ui32 testSize, ECollisionProbability collisionProbability) {
        ui32 valueRange = getValueRange(testSize, collisionProbability);

        DataType *result = new DataType[testSize];
        generateRange(result, result + testSize, RandomFactory<DataType>(), valueRange);
        return result;
    }

};

#endif
