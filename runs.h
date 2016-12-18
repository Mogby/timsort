#pragma once

#ifndef RUN_H
#define RUN_H

template <class RandomAccessIterator>
struct RunInfo {
    RandomAccessIterator begin;
    ui32 size;

    RunInfo() {}

    RunInfo(RandomAccessIterator begin, ui32 size) : begin(begin), size(size) {}
};

template <class RandomAccessIterator>
class RunStack {
typedef RunInfo<RandomAccessIterator> RunType;
private:

    ui32 capacity;
    ui32 size;
    RunType *vector;

    void shrink() {
        if (capacity <= 4)
            return;

        capacity /= 4;
        if (capacity < 4) {
            capacity = 4;
        }

        RunType *newVector = new RunType[capacity];

        for (ui32 pointer = 0; pointer < size; ++pointer) {
            newVector[pointer] = vector[pointer];
        }

        delete[] vector;
        vector = newVector;
    }

    void expand() {
        capacity *= 2;
        RunType *newVector = new RunType[capacity];

        for (ui32 pointer = 0; pointer < size; ++pointer) {
            newVector[pointer] = vector[pointer];
        }

        delete[] vector;
        vector = newVector;
    }

public:

    RunStack() {
        capacity = 4;
        size = 0;
        vector = new RunType[capacity];
    }

    void push(RunType runInfo) {
        vector[size++] = runInfo;

        if (2 * size > capacity) {
            expand();
        }
    }

    void emplace(RandomAccessIterator runBegin, ui32 runSize) {
        push(RunInfo<RandomAccessIterator>(runBegin, runSize));
    }

    void pop() {
        --size;

        if (4 * size < capacity) {
            shrink();
        }
    }

    ui32 getLastThreeRuns(RunType &runX, RunType &runY, RunType &runZ) {
        if (size > 0) {
            runX = vector[size - 1];

            if (size > 1) {
                runY = vector[size - 2];

                if (size > 2) {

                    runZ = vector[size - 3];
                }
            }
        }

        if (size > 3) {
            return 3;
        }

        return size;
    }

    virtual ~RunStack() {
        delete[] vector;
    }
};
#endif
