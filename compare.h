#pragma once

#ifndef COMPARE_H
#define COMPARE_H
template<class ValueType>
struct LessCompare {
    bool operator()(const ValueType &first, const ValueType &second) {
        return first < second;
    }
};

template <class RandomAccessIterator, class Compare>
inline bool areEqual(RandomAccessIterator first, RandomAccessIterator second, Compare comp) {
    return !comp(*first, *second) && !comp(*second, *first);
}
#endif
