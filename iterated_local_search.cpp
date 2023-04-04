#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <thread>

#include "utils.hpp"
#include "best_from_input.hpp"

#ifdef DEBUG
  #define LOG(x) std::cerr << x << std::endl
#else
  #define LOG(x)
#endif

const int subsidiary_deadline_in_ms = 2'000;
const int deadline_in_ms = 60'000;

static inline std::chrono::time_point<std::chrono::high_resolution_clock> now() {
  return std::chrono::high_resolution_clock::now();
}

auto pick_best_2_swap(
  const std::vector<std::vector<int>>& datasets,
  const int n,
  std::vector<int> candidate,
  const uint64_t current_best
) -> std::tuple<int, int, uint64_t> {
  std::pair<int, int> to_swap = { -1, -1 };
  uint64_t global_min = current_best;
  std::vector<int> mapping(n + 1, 0);
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      std::swap(candidate[i], candidate[j]);
      for (int k = 0; k < n; ++k) {
        mapping[candidate[k]] = k + 1;
      }
      uint64_t total_distance = 0ULL;
      for (const auto& data : datasets) {
        total_distance += utils::compute_ulam(mapping, data);
      }
      if (total_distance < global_min) {
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

  auto subsidiary_deadline = now() + std::chrono::milliseconds(subsidiary_deadline_in_ms);
  while (now() < subsidiary_deadline) {
    const auto [i, j, new_best] = pick_best_2_swap(datasets, n, permutation, total_distance);
    if (new_best >= total_distance) {
      break;
    }
    total_distance = new_best;
    std::swap(permutation[i], permutation[j]);
  }

  size_t subsidiary_local_searches = 1;
  while (now() < deadline) {
    subsidiary_local_searches++;
    subsidiary_deadline = now() + std::chrono::milliseconds(subsidiary_deadline_in_ms);

    // Do a pertubation, by doing 2 random swaps mimicing 4-exchange in TSP
    std::vector<int> local_permutation = permutation;
    std::swap(local_permutation[rand_int(rng)], local_permutation[rand_int(rng)]);
    std::swap(local_permutation[rand_int(rng)], local_permutation[rand_int(rng)]);
    auto [local_mapping, local_total_distance] = utils::compute_distance(datasets, local_permutation, n);

    // Do a subsidiary local search
    while (now() < subsidiary_deadline) {
      const auto [i, j, new_best] = pick_best_2_swap(datasets, n, local_permutation, local_total_distance);
      if (new_best >= local_total_distance) {
        break;
      }
      local_total_distance = new_best;
      std::swap(local_permutation[i], local_permutation[j]);
    }

    if (local_total_distance < total_distance) {
      permutation = local_permutation;
      total_distance = local_total_distance;
    }
  }

  LOG("Number of subsidiary local searches " << subsidiary_local_searches);
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
