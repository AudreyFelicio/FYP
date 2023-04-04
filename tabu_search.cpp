#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <thread>
#include <deque>

#include "utils.hpp"
#include "best_from_input.hpp"

#ifdef DEBUG
  #define LOG(x) std::cerr << x << std::endl
#else
  #define LOG(x)
#endif

const int deadline_in_ms = 60'000;
const int tabu_tenure = 7;
const double acceptance_threshold = 0.7;

static inline std::chrono::time_point<std::chrono::high_resolution_clock> now() {
  return std::chrono::high_resolution_clock::now();
}

auto pick_improving_or_best_2_swap(
  const std::vector<std::vector<int>>& datasets,
  const int n,
  std::vector<int> candidate,
  const uint64_t current_best,
  std::deque<std::pair<int, int>>& tabu_table
) -> std::tuple<int, int, uint64_t> {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<std::mt19937::result_type> rand_int(1, n);

  std::pair<int, int> to_swap = { -1, -1 };
  uint64_t global_min = current_best;
  std::vector<int> mapping(n + 1, 0);
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (std::find(tabu_table.begin(), tabu_table.end(), std::make_pair(i, j)) != tabu_table.end()) continue;
      std::swap(candidate[i], candidate[j]);
      for (int k = 0; k < n; ++k) {
        mapping[candidate[k]] = k + 1;
      }
      uint64_t total_distance = 0ULL;
      for (const auto& data : datasets) {
        total_distance += utils::compute_ulam(mapping, data);
      }
      if (total_distance < global_min) {
        if (acceptance_threshold * n >= rand_int(rng)) {
          return { i, j, total_distance };
        }
        to_swap = { i, j };
        global_min = total_distance;
      }
      std::swap(candidate[i], candidate[j]);
    }
  }

  return { to_swap.first, to_swap.second, global_min };
}

template<typename T>
auto local_search(
  const std::vector<std::vector<int>> datasets, 
  const int n, 
  const std::chrono::time_point<T> deadline,
  std::pair<std::vector<int>, uint64_t>& output
) -> void {
  std::vector<int> permutation = algo::best_from_input(datasets, n).first;
  auto [mapping, total_distance] = utils::compute_distance(datasets, permutation, n);
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<std::mt19937::result_type> rand_int(1, n);
  std::deque<std::pair<int, int>> tabu_table;

  size_t iterations = 1;
  while (now() < deadline) {
    iterations++;
    const auto [i, j, new_best] = pick_improving_or_best_2_swap(datasets, n, permutation, total_distance, tabu_table);
    if (new_best >= total_distance) {
      continue;
    }
    total_distance = new_best;
    std::swap(permutation[i], permutation[j]);
    if (tabu_table.size() == tabu_tenure) {
      tabu_table.pop_front();
    }
    tabu_table.push_back({ i, j });
  }

  LOG("Number of iterations: " << iterations);
  output = { permutation, total_distance };
}

auto main() -> int {
  utils::fast_io();

  int n, k;
  const auto datasets = utils::read_input(n, k);

  const auto processor_count = static_cast<int>(std::thread::hardware_concurrency());

  std::vector<std::thread> local_searches;
  std::vector<std::pair<std::vector<int>, uint64_t>> outputs(processor_count);
  for (auto i = 0; i < processor_count; ++i) {
    local_searches.emplace_back(
      std::thread(local_search<std::chrono::high_resolution_clock>, datasets, n, now() + std::chrono::milliseconds(deadline_in_ms), std::ref(outputs[i]))
    );
  }
  for (auto i = 0; i < processor_count; ++i) {
    local_searches[i].join();
  }

  const auto [best_permutation, best_distance] = *std::min_element(outputs.begin(), outputs.end(), [](const auto& p1, const auto& p2) -> bool {
    return p1.second < p2.second;
  });
  std::cout << best_distance << std::endl;
  utils::print_vector<int>(best_permutation);
}
