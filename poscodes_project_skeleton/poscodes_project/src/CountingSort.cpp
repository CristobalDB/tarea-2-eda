#include "CountingSort.hpp"
#include "LinkedList.hpp"

void CountingSort(std::vector<int>& idx, const Poscode* A, std::size_t n, int p, int M) {
    std::vector<LinkedList> buckets(static_cast<std::size_t>(M));

    // 1) Distribuir: O(n)
    for (std::size_t k = 0; k < n; ++k) {
        int i   = idx[k];            // índice al arreglo A
        int key = A[i].getValue(p);  // 0..M-1
        if (key < 0)   key = 0;
        if (key >= M)  key = M - 1;
        buckets[static_cast<std::size_t>(key)].push_back(i); // inserción O(1)
    }

    // 2) Recolectar estable: O(n)
    std::size_t t = 0;
    for (int b = 0; b < M; ++b) {
        for (int v : buckets[static_cast<std::size_t>(b)]) {
            idx[t++] = v;
        }
    }
    // buckets se destruye; sus destructores liberan memoria.
}
