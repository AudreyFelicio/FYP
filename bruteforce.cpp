#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <numeric>
#include <thread>

#include "utils.hpp"

auto parallel_bruteforce(
  const std::vector<std::vector<int>> datasets,
  std::vector<int> permutation,
  const int n,
  std::pair<std::vector<int>, uint64_t>& output
) -> void {
  std::vector<int> global_permutation;
  std::vector<int> mapping(n + 1, 0);
  uint64_t global_min = ULLONG_MAX;
  do {
    for (auto i = 0; i < n; ++i) {
      mapping[permutation[i]] = i + 1;
    }
    uint64_t total_distance = 0ULL;
    for (const auto& data : datasets) {
      total_distance += utils::compute_ulam(mapping, data);
    }
    if (total_distance < global_min) {
      global_min = total_distance;
      global_permutation = permutation;
    }
  } while (std::next_permutation(permutation.begin() + 1, permutation.end()));

  output = { global_permutation, global_min };
}

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  std::vector<int> permutation(n, 0);
  iota(permutation.begin(), permutation.end(), 1);
  std::vector<std::thread> local_bruteforces;
  std::vector<std::pair<std::vector<int>, uint64_t>> outputs(n);
  for (auto i = 0; i < n; ++i) {
    std::reverse(permutation.begin(), permutation.begin() + i + 1);
    std::reverse(permutation.begin() + 1, permutation.begin() + i + 1);
    local_bruteforces.emplace_back(
      std::thread(parallel_bruteforce, datasets, permutation, n, std::ref(outputs[i]))
    );
    std::reverse(permutation.begin() + 1, permutation.begin() + i + 1);
    std::reverse(permutation.begin(), permutation.begin() + i + 1);
  }
  for (auto i = 0; i < n; ++i) {
    local_bruteforces[i].join();
  }

  const auto [best_permutation, best_distance] = *std::min_element(outputs.begin(), outputs.end(), [](const auto& p1, const auto& p2) -> bool {
    return p1.second < p2.second;
  });
  std::cout << best_distance << std::endl;
  utils::print_vector<int>(best_permutation);
  std::cout.flush();
}
