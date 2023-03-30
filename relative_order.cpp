#include <vector>
#include <unordered_set>
#include <functional>
#include <queue>

#include "utils.hpp"

auto toposort(const std::vector<std::vector<int>>& graph, const int n) -> std::vector<int> {
  std::vector<int> sorted;
  std::vector<bool> visited(n + 1, false);
  std::function<void(int)> dfs = [&](const int vertex) -> void {
    visited[vertex] = true;
    for (const auto neighbour : graph[vertex]) {
      if (visited[neighbour]) continue;
      dfs(neighbour);
    }
    sorted.push_back(vertex);
  };

  for (int i = 1; i <= n; ++i) {
    if (!visited[i]) dfs(i);
  }
  return sorted;
}

auto find_minimum_cycle(const std::vector<std::vector<int>>& graph, const int n) -> std::unordered_set<int> {
  std::unordered_set<int> min_cycle;

  for (int i = 1; i <= n; ++i) {
    // find the minimum cycle starting from i
    std::vector<int> parent(n + 1, -1);
    std::vector<bool> visited(n + 1, false);
    std::queue<std::pair<int, int>> bfs;
    bfs.push({ i, 0 });
    visited[i] = true;
    while (!bfs.empty()) {
      const auto [curr_vertex, path_length] = bfs.front(); bfs.pop();
      for (const auto neighbour : graph[curr_vertex]) {
        if (visited[neighbour]) {
          // we find a cycle
          if (path_length + 1 >= static_cast<int>(min_cycle.size())) continue;
          int backtrack = curr_vertex;
          min_cycle = {};
          while (backtrack != -1) {
            min_cycle.insert(backtrack);
            backtrack = parent[backtrack];
          }
        } else {
          parent[neighbour] = curr_vertex;
          bfs.push({ neighbour, path_length + 1 });
          visited[neighbour] = true;
        }
      }
    }
  }

  return min_cycle;
}

auto relative_order(const std::vector<std::vector<int>> datasets, const double alpha, const int n) -> void {
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
  const auto cycle = find_minimum_cycle(adjacency_list, n);
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
  }

  auto permutation = toposort(adjacency_list, n);
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

  std::cout << total_distance << std::endl;
  utils::print_vector<int>(permutation);
}

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  relative_order(datasets, 0.1, n);
}
