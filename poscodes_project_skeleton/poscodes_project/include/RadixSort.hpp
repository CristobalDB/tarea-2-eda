#pragma once
#include <vector>
#include <cstddef>
#include "poscode.hpp"

void RadixSort(std::vector<int>& idx, const Poscode* A, std::size_t n);

void RadixSortColumns(std::vector<int>& idx, const Poscode* A, std::size_t n,
                      const std::vector<int>& cols, const std::vector<int>& Mvals);
