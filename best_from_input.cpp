#include <iostream>
#include <climits>

#include "utils.hpp"

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  uint64_t global_min = ULLONG_MAX;
  std::vector<int> mapping(n + 1, 0);
  int best_index = 0;
  int index = 0;
  for (const auto& data : datasets) {
    uint64_t total_distance = 0ULL;
    for (auto i = 0; i < n; ++i) {
      mapping[data[i]] = i + 1;
    }
    for (const auto& other_data : datasets) {
      total_distance += utils::compute_ulam(mapping, other_data);
    }
    if (total_distance < global_min) {
      global_min = total_distance;
      best_index = index;
    }
    index++;
  }

  std::cout << global_min << std::endl;
  utils::print_vector<int>(datasets[best_index]);
}
