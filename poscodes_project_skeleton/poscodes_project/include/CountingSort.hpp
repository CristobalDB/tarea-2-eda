#pragma once
#include <vector>
#include <cstddef>
#include "poscode.hpp"

// CountingSort estable sobre índices 'idx', usando columna 'p' y rango 'M'
// p en [1..6], M=10 para dígitos, M=26 para letras
void CountingSort(std::vector<int>& idx, const Poscode* A, std::size_t n, int p, int M);
