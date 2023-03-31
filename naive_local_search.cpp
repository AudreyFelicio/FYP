#include <vector>
#include <chrono>
#include <algorithm>
#include <random>

#include "utils.hpp"

#ifdef DEBUG
  #define LOG(x) std::cerr << x << std::endl
#else
  #define LOG(x)
#endif

static inline std::chrono::time_point<std::chrono::high_resolution_clock> now() {
  return std::chrono::high_resolution_clock::now();
}

auto pick_best_2_swap(
  const std::vector<std::vector<int>> datasets,
  const int n,
  std::vector<int> candidate,
  const uint64_t current_best
) -> std::tuple<int, int, uint64_t> {
  std::pair<int, int> to_swap = { -1, -1 };
  uint64_t global_min = current_best;
  std::vector<int> mapping(n + 1, 0);
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      std::swap(candidate[i], candidate[j]);
      for (auto i = 0; i < n; ++i) {
        mapping[candidate[i]] = i + 1;
      }
      uint64_t total_distance = 0ULL;
      for (const auto& data : datasets) {
        total_distance += utils::compute_ulam(mapping, data);
      }
      if (total_distance < global_min) {
        to_swap = { i, j };
        global_min = total_distance;
      }
      std::swap(candidate[i], candidate[j]);
    }
  }

  return { to_swap.first, to_swap.second, global_min };
}

template<typename T>
auto local_search(const std::vector<std::vector<int>> datasets, const int n, const std::chrono::time_point<T>& deadline) -> void {
  std::vector<int> permutation(n, 0);
  std::iota(permutation.begin(), permutation.end(), 1);
  std::random_device rd;
  std::mt19937 rng(rd());
  shuffle(permutation.begin(), permutation.end(), rng);
  std::vector<int> mapping(n + 1, 0);
  for (auto i = 0; i < n; ++i) {
    mapping[permutation[i]] = i + 1;
  }
  uint64_t total_distance = 0ULL;
  for (const auto& data : datasets) {
    total_distance += utils::compute_ulam(mapping, data);
  }

  size_t iterations = 0;
  std::chrono::milliseconds totalTime(0);   // Total time spent in main loop.
  std::chrono::milliseconds averageTime(0); // Average main loop iteration time.

  while (now() < deadline) {
    const auto [i, j, new_best] = pick_best_2_swap(datasets, n, permutation, total_distance);
    if (new_best >= total_distance) {
      break;
    }
    std::swap(permutation[i], permutation[j]);
  }

  std::cout << total_distance << std::endl;
  utils::print_vector<int>(permutation);
}

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  local_search(datasets, n, now() + std::chrono::milliseconds(2'000));
}
