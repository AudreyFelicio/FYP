#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>
#include <unordered_set>

namespace utils {

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

auto compute_ulam(const std::vector<int>& mapping, const std::vector<int>& data) -> uint64_t {
  std::vector<int> lcs;
  for (const auto x : data) {
    int mapped = mapping[x];
    auto it = std::lower_bound(lcs.begin(), lcs.end(), mapped);
    if (it == lcs.end()) {
      lcs.push_back(mapped);
    } else {
      *it = mapped;
    }
  }
  return data.size() - lcs.size();
}

auto fast_io() -> void {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
}

auto read_input(int& n, int& k) -> std::vector<std::vector<int>> {
  std::cin >> n >> k;

  std::vector<std::vector<int>> datasets(k, std::vector<int>(n));
  for (auto i = 0; i < k; ++i) {
    for (auto j = 0; j < n; ++j) {
      std::cin >> datasets[i][j];
    }
  }

  return move(datasets);
}

template<typename T>
auto print_vector(const std::vector<T>& vec) {
  for (const auto& v : vec) {
    std::cout << v << " ";
  }
  std::cout << std::endl;
}

};

#endif // __UTILS_HPP__
