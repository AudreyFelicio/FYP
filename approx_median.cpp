#include <vector>

#include "utils.hpp"
#include "approx_median.hpp"

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  const auto [permutation, distance] = algo::approx_median(datasets, n);

  std::cout << distance << std::endl;
  utils::print_vector<int>(permutation);
}
