#include <vector>
#include <iostream>

#include "utils.hpp"
#include "best_from_input.hpp"
#include "relative_order.hpp"

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  const auto [relative_permutation, relative_distance] = algo::relative_order(datasets, 0.1, n);
  const auto [best_permutation, best_distance] = algo::best_from_input(datasets, n);
  if (relative_distance <= best_distance) {
    std::cout << relative_distance << std::endl;
    utils::print_vector<int>(relative_permutation);
  } else {
    std::cout << best_distance << std::endl;
    utils::print_vector<int>(best_permutation);
  }
}
