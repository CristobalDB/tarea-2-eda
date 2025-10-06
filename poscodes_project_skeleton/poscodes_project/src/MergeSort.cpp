#include "MergeSort.hpp"
#include <vector>
#include <algorithm>

namespace {
inline bool lessByAll(const Poscode& a, const Poscode& b) {
    return Poscode::lessLex(a, b);
}

void mergeRange(std::vector<int>& idx, std::vector<int>& tmp, const Poscode* A, int l, int m, int r) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        // <= para estabilidad: si son iguales, toma el de la izquierda primero
        if (lessByAll(A[idx[i]], A[idx[j]]) || (!lessByAll(A[idx[j]], A[idx[i]]))) {
            tmp[k++] = idx[i++];
        } else {
            tmp[k++] = idx[j++];
        }
    }
    while (i <= m) tmp[k++] = idx[i++];
    while (j <= r) tmp[k++] = idx[j++];
    for (int t = l; t <= r; ++t) idx[t] = tmp[t];
}

void msort(std::vector<int>& idx, std::vector<int>& tmp, const Poscode* A, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    msort(idx, tmp, A, l, m);
    msort(idx, tmp, A, m + 1, r);
    // Optimización: si ya está ordenado, omite merge
    if (!lessByAll(A[idx[m + 1]], A[idx[m]])) return;
    mergeRange(idx, tmp, A, l, m, r);
}
} // namespace

void MergeSort(std::vector<int>& idx, const Poscode* A, std::size_t n) {
    if (n < 2) return;
    std::vector<int> tmp(n);
    msort(idx, tmp, A, 0, static_cast<int>(n) - 1);
}
