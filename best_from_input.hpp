#ifndef __BEST_FROM_INPUT_HPP__
#define __BEST_FROM_INPUT_HPP__

#include <iostream>
#include <climits>
#include <vector>

#include "utils.hpp"

namespace algo {

auto best_from_input(const std::vector<std::vector<int>> datasets, const int n) -> std::pair<std::vector<int>, uint64_t> {
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

  return { datasets[best_index], global_min };
}

} // namespace algo

#endif // __BEST_FROM_INPUT_HPP__
