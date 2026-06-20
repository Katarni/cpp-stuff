#pragma once

#include <vector>

constexpr std::size_t kTaskNum = 10000;

const inline std::vector<std::pair<std::size_t, std::size_t>>
  threads_nums({{1, 1}, {1, 2}, {2, 2}, {2, 4}, {4, 4}, {10, 10}});
