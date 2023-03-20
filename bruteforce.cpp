#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <numeric>

auto fast_io() -> void {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
}

auto compute_lcs(const std::vector<int>& mapping, const std::vector<int>& data) -> uint64_t {
  std::vector<int> lcs;
  for (const auto x : data) {
    int mapped = mapping[x];
    auto it = std::lower_bound(lcs.begin(), lcs.end(), mapped);
    if (it == lcs.end()) {
      lcs.push_back(x);
    } else {
      *it = x;
    }
  }
  return lcs.size();
}

auto main() -> int {
  fast_io();

  int n, k;
  std::cin >> n >> k;

  std::vector<std::vector<int>> datasets(k, std::vector<int>(n));
  for (auto i = 0; i < k; ++i) {
    for (auto j = 0; j < n; ++j) {
      std::cin >> datasets[i][j];
    }
  }

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
      total_distance += compute_lcs(mapping, data);
    }
    global_min = std::min(total_distance, global_min);
  } while (std::next_permutation(permutation.begin(), permutation.end()));
  std::cout << global_min << std::endl;
}
