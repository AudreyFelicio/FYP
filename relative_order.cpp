#include <vector>

#include "utils.hpp"
#include "best_from_input.hpp"

auto relative_order(const std::vector<std::vector<int>> datasets, const double alpha, const int n) -> std::pair<std::vector<int>, uint64_t> {
  // Build graph H
  std::vector<bool> is_removed(n + 1, false);
  std::vector<std::vector<int>> adjacency_list(n + 1, std::vector<int>{});
  for (int i = 1; i <= n; ++i) {
    std::vector<int> frequency(n + 1, 0);
    for (const auto& data : datasets) {
      bool i_found = false;
      for (const auto j : data) {
        // add only if i appears before j
        if (i_found) {
          frequency[j]++;
          continue;
        }
        if (j == i) {
          i_found = true;
        }
      }
    }
    for (int j = 1; j <= n; ++j) {
      if (frequency[j] >= (1.0 - 2 * alpha) * static_cast<double>(datasets.size())) {
        adjacency_list[i].push_back(j);
      }
    }
  }

  // Repeatedly clear out cycles
  auto cycle = utils::find_minimum_cycle(adjacency_list, n);
  while (!cycle.empty()) {
    std::vector<std::vector<int>> new_graph(n + 1, std::vector<int>{});
    for (int i = 1; i <= n; ++i) {
      if (cycle.find(i) != cycle.end()) continue;
      for (auto vertex : adjacency_list[i]) {
        if (cycle.find(vertex) != cycle.end()) continue;
        new_graph[i].push_back(vertex);
      }
    }
    for (const auto vertex : cycle) {
      is_removed[vertex] = true;
    }
    swap(adjacency_list, new_graph);
    cycle = utils::find_minimum_cycle(adjacency_list, n);
  }

  auto permutation = utils::toposort(adjacency_list, n);
  // add missing vertices
  for (int i = 1; i <= n; ++i) {
    if (is_removed[i]) permutation.push_back(i);
  }

  // Compute distance to dataset
  std::vector<int> mapping(n + 1, 0);
  for (auto i = 0; i < n; ++i) {
    mapping[permutation[i]] = i + 1;
  }
  uint64_t total_distance = 0ULL;
  for (const auto& data : datasets) {
    total_distance += utils::compute_ulam(mapping, data);
  }

  return { permutation, total_distance };
}

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  const auto [relative_permutation, relative_distance] = relative_order(datasets, 0.1, n);
  const auto [best_permutation, best_distance] = algo::best_from_input(datasets, n);
  if (relative_distance <= best_distance) {
    std::cout << relative_distance << std::endl;
    utils::print_vector<int>(relative_permutation);
  } else {
    std::cout << best_distance << std::endl;
    utils::print_vector<int>(best_permutation);
  }
}
