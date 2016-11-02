#pragma once

#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H
template <class RandomAccessIterator, class Compare>
void insertionSort(RandomAccessIterator begin, RandomAccessIterator end, Compare comp) {
    if (begin == end)
        return;

    for (RandomAccessIterator pointer = begin + 1; pointer != end; ++pointer) {
        RandomAccessIterator insertPosition = pointer;

        while (insertPosition != begin && comp(*insertPosition, *(insertPosition - 1))) {
            swapElements(*insertPosition, *(insertPosition - 1));
            --insertPosition;
        }
    }
}
#endif
