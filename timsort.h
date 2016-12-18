#pragma once

#ifndef TIMSORT_H
#define TIMSORT_H

#include <iterator>

typedef unsigned int ui32;

#include "timsort_params.h"
#include "runs.h"
#include "compare.h"
#include "block_algorithms.h"
#include "insertion_sort.h"
#include "inplace_merge.h"

template <class RandomAccessIterator, class Compare>
void mergeXY(RunInfo<RandomAccessIterator> runX, RunInfo<RandomAccessIterator> runY,
        RunStack<RandomAccessIterator> &runs, Compare comp, const ITimSortParams &params) {
    inplaceMerge(runY.begin, runX.begin, runX.begin + runX.size, comp, params.GetGallop());
    runs.pop();
    runs.pop();
    runs.emplace(runY.begin, runY.size + runX.size);
}

template <class RandomAccessIterator, class Compare>
void mergeYZ(RunInfo<RandomAccessIterator> runX, RunInfo<RandomAccessIterator> runY,
        RunInfo<RandomAccessIterator> runZ, RunStack<RandomAccessIterator> &runs, 
        Compare comp, const ITimSortParams &params) {
    runs.pop();
    mergeXY(runY, runZ, runs, comp, params);
    runs.emplace(runX.begin, runX.size);
}

template <class RandomAccessIterator, class Compare>
void supportInvariant(RunStack<RandomAccessIterator> &runs,
        Compare comp, const ITimSortParams &params) {
    RunInfo<RandomAccessIterator> runX, runY, runZ;

    bool needMerge = true;
    ui32 runsCount = runs.getLastThreeRuns(runX, runY, runZ);
    while (runsCount >= 3 && needMerge) {
        switch (params.whatMerge(runX.size, runY.size, runZ.size)) {
            case WM_MergeXY:
                mergeXY(runX, runY, runs, comp, params);
                break;
            case WM_MergeYZ:
                mergeYZ(runX, runY, runZ, runs, comp, params);
               break;
            case WM_NoMerge:
                needMerge = false;
                break;
        }

        runsCount = runs.getLastThreeRuns(runX, runY, runZ);
    } 

    if (runsCount == 2) {
        if (params.needMerge(runX.size, runY.size)) {
            mergeXY(runX, runY, runs, comp, params);
        }
    }
}

template <class RandomAccessIterator, class Compare>
void splitArrayIntoRuns(RandomAccessIterator begin, RandomAccessIterator end, Compare comp,
        RunStack<RandomAccessIterator> &runs, const ITimSortParams &params) {
    ui32 minrun = params.minRun(end - begin);

    for (RandomAccessIterator runBegin = begin; runBegin != end;) {
        RandomAccessIterator runEnd = runBegin + 1;

        while (runEnd != end && areEqual(runEnd, runEnd - 1, comp)) {
            ++runEnd;
        }

        bool isDescending = (runEnd == end ? false : comp(*runEnd, *runBegin));

        while (runEnd != end && 
                ((isDescending ? comp(*runEnd, *(runEnd - 1)) : comp(*(runEnd - 1), *runEnd)) ||
            areEqual(runEnd, runEnd - 1, comp))) {
            ++runEnd;
        }

        if (isDescending) {
            reverseBlock(runBegin, runEnd);
        }

        while (runEnd != end && runEnd - runBegin < minrun) {
            ++runEnd;
        }

        insertionSort(runBegin, runEnd, comp);

        runs.emplace(runBegin, runEnd - runBegin);

        supportInvariant(runs, comp, params);

        runBegin = runEnd;
    }
}

template <class RandomAccessIterator, class Compare>
void mergeRuns(RunStack<RandomAccessIterator> &runs, Compare comp,
        const ITimSortParams &params) {
    RunInfo<RandomAccessIterator> runX, runY, runZ;
    ui32 runsCount = runs.getLastThreeRuns(runX, runY, runZ);

    while (runsCount > 1) {
        inplaceMerge(runY.begin, runX.begin, runX.begin + runX.size, comp, params.GetGallop());

        runs.pop();
        runs.pop();
        runs.emplace(runY.begin, runY.size + runX.size);

        runsCount = runs.getLastThreeRuns(runX, runY, runZ);
    }
}

template <class RandomAccessIterator, class Compare>
void timSort(RandomAccessIterator begin, RandomAccessIterator end, Compare comp, 
        const ITimSortParams &params = DefaultParams()) {

    RunStack<RandomAccessIterator> runs;

    splitArrayIntoRuns(begin, end, comp, runs, params);

    mergeRuns(runs, comp, params);

}

template <class RandomAccessIterator>
void timSort(RandomAccessIterator first, RandomAccessIterator last,
        const ITimSortParams &params = DefaultParams()) {
    timSort(first, last, LessCompare<typename std::iterator_traits<RandomAccessIterator>::value_type>(), params);
}


#endif
