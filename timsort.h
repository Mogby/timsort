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
void timSort(RandomAccessIterator begin, RandomAccessIterator end, Compare comp, 
        const ITimSortParams &params = DefaultParams()) {
    //initialization
    ui32 minrun = params.minRun(end - begin);
    ui32 runsCount;
    RunStack<RandomAccessIterator> runs;
    RunInfo<RandomAccessIterator> runX, runY, runZ;

    //splitting array into runs
    for (RandomAccessIterator runBegin = begin; runBegin != end;) {
        RandomAccessIterator runEnd = runBegin + 1;

        while (runEnd != end && areEqual(runEnd, runEnd - 1, comp)) {
            ++runEnd;
        }

        bool isDescending;
        if (runEnd == end) {
            isDescending = false;
        } else {
            isDescending = comp(*runEnd, *runBegin);
        }

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

        bool needMerge = true;
        ui32 runsCount = runs.getLastThreeRuns(runX, runY, runZ);
        while (runsCount >= 3 && needMerge) {
            switch (params.whatMerge(runX.size, runY.size, runZ.size)) {
                case WM_MergeXY:
                    inplaceMerge(runY.begin, runX.begin, runX.begin + runX.size, comp, params.GetGallop());
                    runs.pop();
                    runs.pop();
                    runs.emplace(runY.begin, runY.size + runX.size);
                    break;
                case WM_MergeYZ:
                    inplaceMerge(runZ.begin, runY.begin, runY.begin + runY.size, comp, params.GetGallop());
                    runs.pop();
                    runs.pop();
                    runs.pop();
                    runs.emplace(runZ.begin, runZ.size + runY.size);
                    runs.emplace(runX.begin, runX.size);
                    break;
                case WM_NoMerge:
                    needMerge = false;
                    break;
            }

            runsCount = runs.getLastThreeRuns(runX, runY, runZ);
        } 
        
        if (runsCount == 2) {
            if (params.needMerge(runX.size, runY.size)) {
                inplaceMerge(runY.begin, runX.begin, runX.begin + runX.size, comp, params.GetGallop());
                runs.pop();
                runs.pop();
                runs.emplace(runY.begin, runY.size + runX.size);
            }
        }

        runBegin = runEnd;
    }

    runsCount = runs.getLastThreeRuns(runX, runY, runZ);
    //merging runs
    while (runsCount > 1) {
        inplaceMerge(runY.begin, runX.begin, runX.begin + runX.size, comp, params.GetGallop());

        runs.pop();
        runs.pop();
        runs.emplace(runY.begin, runY.size + runX.size);

        runsCount = runs.getLastThreeRuns(runX, runY, runZ);
    }

}

template <class RandomAccessIterator>
void timSort(RandomAccessIterator first, RandomAccessIterator last,
        const ITimSortParams &params = DefaultParams()) {
    timSort(first, last, LessCompare<typename std::iterator_traits<RandomAccessIterator>::value_type>(), params);
}


#endif
