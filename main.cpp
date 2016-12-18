#include "timsort.h"
//#include "test_sequence.h"
#include "deque.h"
#include <iostream>
#include <cassert>

int main() {
    
    Deque<int> d;

    srand(37);
    std::cout << "DEQUE:\n";
    for (int i = 0; i < 10000; ++i) {
        d.push_back(rand());
        std::cout << d[i] << ' ';
    }
    std::cout << std::endl;

    timSort(d.begin(), d.end(), std::greater<int>());

    std::cout << "SORTED DEQUE?\n";

    for (int i = 0; i < 10000; ++i) {
        std::cout << d[i] << ' ';
        if (i) {
            assert(d[i] >= d[i-1]);
        }
    }

    //runTestSequence();

    return 0;
}
