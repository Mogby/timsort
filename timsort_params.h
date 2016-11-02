#pragma once

#ifndef TIMSORT_PARAMS
#define TIMSORT_PARAMS

enum EWhatMerge {
    WM_NoMerge,
    WM_MergeXY,
    WM_MergeYZ
};

class ITimSortParams {
public:

    virtual ui32 minRun(ui32 coun) const = 0;

    virtual bool needMerge(ui32 lenX, ui32 lenY) const = 0;

    virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const = 0;

    virtual ui32 GetGallop() const = 0;

};

class DefaultParams : public ITimSortParams {
public:

    virtual ui32 minRun(ui32 count) const {
        ui32 addBit = 0;
    
        while (count >= 4) {
            addBit |= count & 1;
            count >>= 1;
        }

        return count + addBit;
    }

    virtual bool needMerge(ui32 lenX, ui32 lenY) const {
        return lenY <= lenX;
    }

    virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const {
        if (lenZ > lenX + lenY && lenY > lenX) {
            return WM_NoMerge;
        } else if (lenX < lenZ) {
            return WM_MergeXY;
        } else {
            return WM_MergeYZ;
        }
    }

    virtual ui32 GetGallop() const {
        return 7;
    }

};

class ChaoticParams : DefaultParams {

    virtual ui32 minRun(ui32 count) const {
        return 1;
    }

};

#endif


