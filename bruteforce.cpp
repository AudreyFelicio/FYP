#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <numeric>

#include "utils.hpp"

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  std::vector<int> global_permutation(n, 0);
  std::vector<int> permutation(n, 0);
  std::iota(permutation.begin(), permutation.end(), 1);
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
  } while (std::next_permutation(permutation.begin(), permutation.end()));

  std::cout << global_min << std::endl;
  utils::print_vector<>(global_permutation);
}
