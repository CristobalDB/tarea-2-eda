#pragma once
#include <vector>
#include <cstddef>
#include "poscode.hpp"

// Ordena los índices 'idx' según el orden lexicográfico de los Poscode en A.
// No es estable. Implementación optimizada con mediana-de-tres e insertion sort para tramos pequeños.
void QuickSort(std::vector<int>& idx, const Poscode* A, std::size_t n);
