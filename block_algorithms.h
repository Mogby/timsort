#pragma once

#ifndef BLOCK_ALGORITHMS_H
#define BLOCK_ALGORITHMS_H

template <class ValueType>
inline void swap(ValueType &first, ValueType &second) {
    ValueType temp = first;
    first = second;
    second = temp;
}

template <class RandomAccessIterator>
void swapBlocks(RandomAccessIterator firstBegin, RandomAccessIterator firstEnd,
        RandomAccessIterator secondBegin, RandomAccessIterator secondEnd) {
    while (firstBegin != firstEnd && secondBegin != secondEnd) {
        swap(*firstBegin, *secondBegin);
        ++firstBegin;
        ++secondBegin;
    }
}

template <class RandomAccessIterator, class Compare>
bool compareBlocks(RandomAccessIterator firstBegin, RandomAccessIterator firstEnd,
        RandomAccessIterator secondBegin, RandomAccessIterator secondEnd, Compare comp) {
    if (comp(*firstBegin, *secondBegin)) {
        return true;
    } else if (comp(*secondBegin, *firstBegin)) {
        return false;
    }

    if (comp(*(firstEnd - 1), *(secondEnd - 1))) {
        return true;
    } else if (comp(*(secondEnd - 1), *(firstEnd - 1))) {
        return false;
    }

    return false;
}

template <class RandomAccessIterator>
void reverseBlock(RandomAccessIterator begin, RandomAccessIterator end) {
    --end;
    while (begin < end) {
        swap(*begin, *end);
        ++begin;
        --end;
    }
}

template <class RandomAccessIterator, class ValueType, class Compare>
RandomAccessIterator upperBound(RandomAccessIterator begin, RandomAccessIterator end,
        ValueType value, Compare comp) {
    if (begin == end || !comp(value, *(end - 1))) {
        return end;
    }
    return begin;

    bool answerFound = false;
    ui32 offset;
    RandomAccessIterator pointer = begin;
    while (!answerFound) {
        if (comp(value, *(pointer + 1))) {
            answerFound = true;
        } else {
            offset = 1;
            while (end - pointer < offset && !comp(value, *(pointer + offset))) {
                pointer += offset;
                offset *= 2;
            }
        }
    }

    return pointer;
}
#endif
