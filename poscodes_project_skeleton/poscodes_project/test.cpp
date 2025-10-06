
#include "poscode.hpp"
#include "utils.hpp"
#include "RadixSort.hpp"
#include "QuickSort.hpp"
#include "MergeSort.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <optional>
#include <random>
#include <string>
#include <tuple>
#include <vector>

namespace fs = std::filesystem;

struct Stats {
    double mean;
    double stddev;
};

static Stats compute_stats(const std::vector<double>& xs) {
    if (xs.empty()) return {0.0, 0.0};
    double m = std::accumulate(xs.begin(), xs.end(), 0.0) / static_cast<double>(xs.size());
    double s2 = 0.0;
    for (double v : xs) {
        double d = v - m;
        s2 += d*d;
    }
    double sd = std::sqrt(s2 / static_cast<double>(xs.size()));
    return {m, sd};
}

// Detecta la raíz del proyecto: <exe>/.. (si ejecutas ./build/test)
static fs::path detect_project_root(char** argv) {
    fs::path exe = fs::canonical(fs::path(argv[0]));
    fs::path maybe = exe.parent_path().parent_path(); // build -> project
    if (fs::exists(maybe / "CMakeLists.txt")) return maybe;
    // fallback: current path
    return fs::current_path();
}

static std::optional<fs::path> find_dataset(const fs::path& root, const std::string& fname) {
    fs::path p1 = root / "codigos" / fname;
    if (fs::exists(p1)) return p1;
    fs::path p2 = root / fname;
    if (fs::exists(p2)) return p2;
    return std::nullopt;
}

// Ejecuta un algoritmo de ordenación sobre 'idx' y mide el tiempo en ms
using SortFn = void(*)(std::vector<int>&, const Poscode*, std::size_t);

static double time_once(SortFn fn, std::vector<int>& idx, const Poscode* A, std::size_t n) {
    Timer t; t.start();
    fn(idx, A, n);
    return t.ms();
}

// Helpers para pasar las funciones con signatura adecuada
static void run_radix(std::vector<int>& idx, const Poscode* A, std::size_t n) { RadixSort(idx, A, n); }
static void run_quick(std::vector<int>& idx, const Poscode* A, std::size_t n) { QuickSort(idx, A, n); }
static void run_merge(std::vector<int>& idx, const Poscode* A, std::size_t n) { MergeSort(idx, A, n); }

struct ResultCell {
    Stats radix;
    Stats quick;
    Stats merge;
    std::size_t n;
    bool ran = false;
};

static ResultCell benchmark_size(const fs::path& file, std::size_t n, int trials = 5, unsigned seed0 = 42) {
    std::cout << "\n=== Dataset: " << file.string() << " (n=" << n << ") ===\n";
    Poscode* A = readCodes(file.string(), n);

    std::vector<int> base_idx(n);
    std::iota(base_idx.begin(), base_idx.end(), 0);

    std::mt19937 rng(seed0);

    auto do_algo = [&](SortFn fn, const char* name) -> Stats {
        std::vector<double> times;
        times.reserve(trials);
        for (int t = 0; t < trials; ++t) {
            std::vector<int> idx = base_idx;
            // Barajar índices para no sesgar Quick/Merge
            std::shuffle(idx.begin(), idx.end(), rng);
            double ms = time_once(fn, idx, A, n);
            times.push_back(ms);
            std::cout << "  " << name << " run " << (t+1) << "/" << trials << ": " << ms << " ms\n";
        }
        auto st = compute_stats(times);
        std::cout << "  -> " << name << " mean ± std: " << st.mean << " ± " << st.stddev << " ms\n";
        return st;
    };

    ResultCell cell;
    cell.n = n;
    cell.radix = do_algo(&run_radix, "Radix");
    cell.quick = do_algo(&run_quick, "Quick");
    cell.merge = do_algo(&run_merge, "Merge");
    cell.ran = true;

    delete[] A;
    return cell;
}

static void print_table(const std::vector<ResultCell>& rows) {
    std::cout << "\n================= RESULTADOS (ms) =================\n";
    std::cout << std::left << std::setw(10) << "Tamano"
              << std::setw(24) << "Radix (µ ± σ)"
              << std::setw(24) << "Quick (µ ± σ)"
              << std::setw(24) << "Merge (µ ± σ)"
              << "\n";
    std::cout << std::string(10+24+24+24, '-') << "\n";

    auto fmt = [](const Stats& s) {
        std::ostringstream oss;
        oss.setf(std::ios::fixed); oss<<std::setprecision(2)<<s.mean<<" ± "<<s.stddev;
        return oss.str();
    };

    for (const auto& r : rows) {
        if (!r.ran) continue;
        std::cout << std::left << std::setw(10) << r.n
                  << std::setw(24) << fmt(r.radix)
                  << std::setw(24) << fmt(r.quick)
                  << std::setw(24) << fmt(r.merge)
                  << "\n";
    }
    std::cout << std::string(10+24+24+24, '-') << "\n";
    std::cout << "Nota: tiempos en milisegundos; 5 corridas por celda (media y desviacion estandar).\n";
}

int main(int argc, char** argv) {
    fs::path root = detect_project_root(argv);
    std::cout << "Raiz del proyecto: " << root.string() << "\n";

    struct DS { const char* fname; std::size_t n; };
    std::array<DS,3> datasets = {{
        {"codes_500K.txt", 500000},
        {"codes_1M.txt",   1000000},
        {"codes_10M.txt",  10000000}
    }};

    std::vector<ResultCell> results;
    results.reserve(datasets.size());

    for (const auto& ds : datasets) {
        auto pathopt = find_dataset(root, ds.fname);
        if (!pathopt) {
            std::cout << ">> Omitiendo " << ds.fname << " (archivo no encontrado en " 
                      << (root/"codigos").string() << ")\n";
            continue;
        }
        try {
            results.push_back(benchmark_size(*pathopt, ds.n, /*trials=*/5, /*seed=*/42));
        } catch (const std::exception& e) {
            std::cerr << "ERROR corriendo " << ds.fname << ": " << e.what() << "\n";
        }
    }

    print_table(results);
    return 0;
}
