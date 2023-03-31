#include <vector>
#include <climits>

#include "utils.hpp"
#include "best_from_input.hpp"

auto median_reconstruct(const std::vector<std::vector<int>>& datasets, const int n) -> std::vector<int> {
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
      if (frequency[j] >= 3) {
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
  return permutation;
}

auto approx_median(const std::vector<std::vector<int>> datasets, const int n) -> void {
  std::vector<std::vector<int>> optimal_candidates = { algo::best_from_input(datasets, n).first };

  const auto m = datasets.size();
  for (int i = 0; i < m; ++i) {
    for (int j = i + 1; j < m; ++j) {
      for (int k = j + 1; k < m; ++k) {
        for (int l = k + 1; l < m; ++l) {
          for (int x = l + 1; x < m; ++x) {
            const auto subset = { datasets[i], datasets[j], datasets[k], datasets[l], datasets[x] };
            optimal_candidates.push_back(move(median_reconstruct(subset, n)));
          }
        }
      }
    }
  }

  uint64_t global_min = ULLONG_MAX;
  std::vector<int> mapping(n + 1, 0);
  std::vector<int> result;
  for (const auto& permutation : optimal_candidates) {
    for (auto i = 0; i < n; ++i) {
      mapping[permutation[i]] = i + 1;
    }
    uint64_t total_distance = 0ULL;
    for (const auto& data : datasets) {
      total_distance += utils::compute_ulam(mapping, data);
    }
    if (total_distance < global_min) {
      global_min = total_distance;
      result = permutation;
    }
  }

  std::cout << global_min << std::endl;
  utils::print_vector<int>(result);
}

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  approx_median(datasets, n);
}
