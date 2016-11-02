#include "timsort.h"
#include "test_sequence.h"

int main() {
    //srand(0451);

    /*ui64 N = 16;
    scanf("%llu", &N);
    int a[N], b[N];
    for (int i = 0; i < N; ++i) {
        a[i] = rand() % N;
        b[i] = a[i];
    }
    timSort(a, a + N);
    std::sort(b, b + N);

    for (int i = 0; i < N; ++i) {
        if (a[i] != b[i]) {
            cout << "FAIL ";
        }
        cout << a[i] << " -- " << b[i] << "\n";
    }*/
    runTestSequence();

    return 0;
}
