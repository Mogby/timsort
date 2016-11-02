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

struct Point3D {
    int x, y, z;

    bool operator<(const Point3D &other) const {
        if (x != other.x) {
            return x < other.x;
        } else if (y != other.y) {
            return y < other.y;
        } else {
            return z < other.z;
        }
    }

    bool operator==(const Point3D &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Point3D &other) const {
        return !(*this == other);
    }

    Point3D() {}
};

template <>
class RandomFactory<Point3D> {
public:

    Point3D generateObject(ui32 range) {
        Point3D result;
        
        int coordinatesRange = 1;
        while (coordinatesRange * coordinatesRange * coordinatesRange < range) {
            ++coordinatesRange;
        }

        result.x = rand() % coordinatesRange;
        result.y = rand() % coordinatesRange;
        result.z = rand() % coordinatesRange;

        return result;
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
