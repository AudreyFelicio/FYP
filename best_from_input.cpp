#include <iostream>

#include "best_from_input.hpp"
#include "utils.hpp"

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  const auto [best_permutation, distance] = algo::best_from_input(datasets, n);
  std::cout << distance << std::endl;
  utils::print_vector<int>(best_permutation);
}
