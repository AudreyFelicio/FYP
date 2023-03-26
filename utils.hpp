#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <vector>
#include <algorithm>

namespace utils {

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
