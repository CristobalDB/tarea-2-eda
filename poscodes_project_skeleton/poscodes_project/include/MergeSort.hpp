#pragma once
#include <vector>
#include <cstddef>
#include "poscode.hpp"

// MergeSort ESTABLE sobre los índices 'idx' según el orden lexicográfico de los Poscode en A.
void MergeSort(std::vector<int>& idx, const Poscode* A, std::size_t n);
