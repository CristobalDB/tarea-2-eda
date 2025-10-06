#include "RadixSort.hpp"
#include "CountingSort.hpp"
#include <stdexcept>

void RadixSortColumns(std::vector<int>& idx, const Poscode* A, std::size_t n,
                      const std::vector<int>& cols, const std::vector<int>& Mvals) {
    if (cols.size() != Mvals.size()) {
        throw std::invalid_argument("cols y Mvals deben tener el mismo tamaño");
    }
    for (std::size_t k = 0; k < cols.size(); ++k) {
        CountingSort(idx, A, n, cols[k], Mvals[k]);
    }
}

void RadixSort(std::vector<int>& idx, const Poscode* A, std::size_t n) {
    if (n <= 1) return;

    CountingSort(idx, A, n, 6, 26);
    CountingSort(idx, A, n, 5, 26);
    CountingSort(idx, A, n, 4, 10);
    CountingSort(idx, A, n, 3, 10);
    CountingSort(idx, A, n, 2, 10);
    CountingSort(idx, A, n, 1, 10);
}
