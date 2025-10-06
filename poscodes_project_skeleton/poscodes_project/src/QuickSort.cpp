#include "QuickSort.hpp"
#include <algorithm>

namespace {
// Comparador lexicográfico por las 6 posiciones
inline bool lessByAll(const Poscode& a, const Poscode& b) {
    return Poscode::lessLex(a, b);
}

constexpr int CUTOFF = 16;

inline void insertionSort(std::vector<int>& idx, const Poscode* A, int lo, int hi) {
    for (int i = lo + 1; i <= hi; ++i) {
        int key = idx[i];
        int j = i - 1;
        while (j >= lo && lessByAll(A[key], A[idx[j]])) {
            idx[j + 1] = idx[j];
            --j;
        }
        idx[j + 1] = key;
    }
}

inline int median3(std::vector<int>& idx, const Poscode* A, int lo, int mid, int hi) {
    // Ordena (lo, mid, hi) parcialmente y deja un buen pivote en mid
    if (lessByAll(A[idx[mid]], A[idx[lo]])) std::swap(idx[lo], idx[mid]);
    if (lessByAll(A[idx[hi ]], A[idx[lo]])) std::swap(idx[lo], idx[hi ]);
    if (lessByAll(A[idx[hi ]], A[idx[mid]])) std::swap(idx[mid], idx[hi ]);
    return mid;
}

int partition(std::vector<int>& idx, const Poscode* A, int lo, int hi) {
    int mid = lo + (hi - lo) / 2;
    int m = median3(idx, A, lo, mid, hi);
    std::swap(idx[m], idx[hi]); // pivot to end
    int pivot = idx[hi];

    int i = lo;
    for (int j = lo; j < hi; ++j) {
        if (lessByAll(A[idx[j]], A[pivot])) {
            std::swap(idx[i], idx[j]);
            ++i;
        }
    }
    std::swap(idx[i], idx[hi]);
    return i;
}

void qsort_rec(std::vector<int>& idx, const Poscode* A, int lo, int hi) {
    while (lo < hi) {
        if (hi - lo + 1 <= CUTOFF) {
            insertionSort(idx, A, lo, hi);
            return;
        }
        int p = partition(idx, A, lo, hi);
        // Tail recursion elimination: sort smaller part first
        if (p - lo < hi - p) {
            qsort_rec(idx, A, lo, p - 1);
            lo = p + 1;
        } else {
            qsort_rec(idx, A, p + 1, hi);
            hi = p - 1;
        }
    }
}

} // namespace

void QuickSort(std::vector<int>& idx, const Poscode* A, std::size_t n) {
    if (n < 2) return;
    qsort_rec(idx, A, 0, static_cast<int>(n) - 1);
}
