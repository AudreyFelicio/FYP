#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

#include "utils.hpp"

auto main() -> int {
  utils::fast_io();

  int n, k;
  std::cin >> n >> k;
  std::cout << n << " " << k << "\n";

  std::vector<int> permutation(n, 0);
  std::iota(permutation.begin(), permutation.end(), 1);
  std::random_device rd;
  std::mt19937 g(rd());

  for (auto i = 0; i < k; ++i) {
    shuffle(permutation.begin(), permutation.end(), g);
    utils::print_vector<>(permutation);
  }
  std::cout.flush();
}
