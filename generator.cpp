#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

auto fast_io() -> void {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
}

auto main() -> int {
  int n, k;
  std::cin >> n >> k;
  std::cout << n << " " << k << "\n";

  std::vector<int> permutation(n, 0);
  std::iota(permutation.begin(), permutation.end(), 1);
  std::random_device rd;
  std::mt19937 g(rd());

  for (auto i = 0; i < k; ++i) {
    shuffle(permutation.begin(), permutation.end(), g);
    for (auto p : permutation) {
      std::cout << p << " ";
    }
    std::cout << "\n";
  }
  std::cout.flush();
}
