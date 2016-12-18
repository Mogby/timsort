#pragma once

#ifndef INPLACE_MERGE_H
#define INPLACE_MERGE_H

template <class RandomAccessIterator, class Compare>
void doGallop(RandomAccessIterator &begin, RandomAccessIterator end,
        RandomAccessIterator compareWith, RandomAccessIterator &destination, Compare comp) {

    RandomAccessIterator rangeEnd = upperBound(begin, end, *compareWith, comp);
    swapBlocks(destination, destination + (rangeEnd - begin), begin, rangeEnd);
    destination += rangeEnd - begin;
    begin = rangeEnd;

}

template <class RandomAccessIterator, class Compare>
void gallopMerge(RandomAccessIterator begin, RandomAccessIterator buffer, 
        ui32 firstBlockLength, ui32 secondBlockLength, ui32 gallop, Compare comp) {
    RandomAccessIterator middle = begin + firstBlockLength;
    RandomAccessIterator end = middle + secondBlockLength;
    RandomAccessIterator bufferEnd = buffer + firstBlockLength;

    swapBlocks(begin, middle, buffer, bufferEnd);
    
    ui32 lastBlock = 0;
    ui32 gallopCount = 0;
    while (middle != end || buffer != bufferEnd) {
        if (middle == end) {
            swapBlocks(begin, end, buffer, bufferEnd);
            buffer = bufferEnd;
        } else if (buffer == bufferEnd) {
            middle = end;
        } else if (comp(*middle, *buffer)) {
            if (lastBlock != 0) {
                lastBlock = 0;
                gallopCount = 0;
            }
            ++gallopCount;

            swapElements(*(begin++), *(middle++));
        } else {
            if (lastBlock != 1) {
                lastBlock = 1;
                gallopCount = 0;
            }
            ++gallopCount;

            swapElements(*(begin++), *(buffer++));
        }

        if (gallopCount == gallop) {
            gallopCount = 0;

            if (lastBlock == 0) {
                doGallop(middle, end, buffer, begin, comp);
            } else {
                doGallop(buffer, bufferEnd, middle, begin, comp);
            }

        }
    }
}

template <class RandomAccessIterator, class Compare>
void gallopMerge(RandomAccessIterator begin, RandomAccessIterator buffer, 
        ui32 blockLength, ui32 gallop, Compare comp) {
    gallopMerge(begin, buffer, blockLength, blockLength, gallop, comp);
}

template <class RandomAccessIterator>
ui32 findBlockLength(RandomAccessIterator begin, RandomAccessIterator end) {
    ui32 blockLength = 1;
    while ((blockLength + 1) * (blockLength + 1) <= end - begin) {
        ++blockLength;
    }

    return blockLength;
}

template <class RandomAccessIterator>
RandomAccessIterator prepareBufferBlock(RandomAccessIterator begin, RandomAccessIterator middle,
        RandomAccessIterator end, ui32 blockLength, ui32 remainingSize) {
    RandomAccessIterator bufferBlock = end;
    for (RandomAccessIterator pointer = begin; end - pointer >= blockLength; pointer += blockLength) {
        if (pointer <= middle && middle < pointer + blockLength) {
            bufferBlock = pointer;
        }
    }

    if (bufferBlock != end) {
        swapBlocks(bufferBlock, bufferBlock + blockLength,
                end - remainingSize, end - remainingSize + blockLength);
    }

    return end - remainingSize;
}

template <class RandomAccessIterator, class Compare>
void sortBlocks(RandomAccessIterator begin, RandomAccessIterator end, Compare comp, ui32 blockLength) {
    RandomAccessIterator minBlock;
    for (RandomAccessIterator insertPosition = begin; insertPosition != end; 
            insertPosition += blockLength) {
        minBlock = insertPosition;

        for (RandomAccessIterator currentBlock = insertPosition + blockLength;
                currentBlock != end; currentBlock += blockLength) {
            if (compareBlocks(currentBlock, currentBlock + blockLength, 
                        minBlock, minBlock + blockLength, comp)) {
                minBlock = currentBlock;
            }
        }

        swapBlocks(minBlock, minBlock + blockLength, insertPosition, insertPosition + blockLength);
    }
}

template <class RandomAccessIterator, class Compare>
void mergeBlocks(RandomAccessIterator begin, RandomAccessIterator end, Compare comp,
        ui32 blockLength, ui32 gallop) {
    for (RandomAccessIterator currentBlock = begin + blockLength; currentBlock != end;
            currentBlock += blockLength) {
        gallopMerge(currentBlock - blockLength, end, blockLength, gallop, comp);
    }
}

template <class RandomAccessIterator, class Compare>
void reverseMergeBlocks(RandomAccessIterator begin, RandomAccessIterator end, 
        RandomAccessIterator bufferBlock, Compare comp, ui32 blockLength, ui32 gallop) {
    if (end - begin >= 3 * blockLength) {
        for (RandomAccessIterator currentBlock = end - 3 * blockLength; currentBlock >= begin;
                currentBlock -= blockLength) {
            gallopMerge(currentBlock, bufferBlock, blockLength, gallop, comp);
        }
    }

    gallopMerge(begin, bufferBlock, (end - begin) % blockLength, blockLength, gallop, comp);

    insertionSort(bufferBlock, bufferBlock + blockLength, comp);
}

template <class RandomAccessIterator, class Compare>
void inplaceMerge(RandomAccessIterator begin, RandomAccessIterator middle, RandomAccessIterator end,
        Compare comp, ui32 gallop) {
    ui32 blockLength = findBlockLength(begin, end);
    ui32 remainingSize = blockLength + (end - begin) % blockLength;

    RandomAccessIterator bufferBlock = prepareBufferBlock(begin, middle, end, blockLength, remainingSize);

    sortBlocks(begin, bufferBlock, comp, blockLength);

    mergeBlocks(begin, bufferBlock, comp, blockLength, gallop);

    //sorting buffer

    if (end - begin <= 2 * remainingSize) {
        insertionSort(begin, end, comp);
        return;
    }

    insertionSort(end - 2 * remainingSize, end, comp);

    reverseMergeBlocks(begin, end, bufferBlock, comp, remainingSize, gallop);
}
#endif
