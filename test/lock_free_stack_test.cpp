#include "lock_free_stack.h"
#include "test_helper.h"

#include <gtest/gtest.h>
#include <vector>

namespace {
std::atomic<int> task_cnt;
std::atomic<int> produced_tasks;;

std::mutex consumed_tasks_mtx;
std::vector<int> consumed_tasks;

void produce(kat_prl::LockFreeStack<int>& stack) {
  for (;;) {
    auto num = task_cnt.load();
    while (num > 0 && !task_cnt.compare_exchange_weak(num, num - 1)) {
      std::this_thread::yield();
    }
    if (num <= 0) {
      return;
    }

    stack.push(num);
    produced_tasks.fetch_add(1);
  }
}

void consume(kat_prl::LockFreeStack<int>& stack) {
  while (!(produced_tasks.load() == kTaskNum && stack.empty())) {
    const auto data = stack.pop();

    if (!data) {
      std::this_thread::yield();
      continue;
    }
    std::lock_guard lock{consumed_tasks_mtx};
    consumed_tasks.push_back(*data);
  }
}
} // namespace

class LockFreeStackTest : public ::testing::TestWithParam<std::pair<std::size_t, std::size_t>> {};

TEST_P(LockFreeStackTest, IntTasksTest) {
  task_cnt = kTaskNum;
  produced_tasks = 0;
  consumed_tasks.clear();

  auto [prods_cnt, cons_cnt] = GetParam();

  std::vector<std::thread> prods;
  std::vector<std::thread> cons;
  kat_prl::LockFreeStack<int> stack;

  for (std::size_t i = 0; i < prods_cnt; ++i) {
    prods.emplace_back(produce, std::ref(stack));
  }

  for (std::size_t i = 0; i < cons_cnt; ++i) {
    cons.emplace_back(consume, std::ref(stack));
  }

  for (auto& t : prods) {
    t.join();
  }
  for (auto& t : cons) {
    t.join();
  }

  ASSERT_EQ(task_cnt, 0);
  ASSERT_EQ(consumed_tasks.size(), kTaskNum);
}

INSTANTIATE_TEST_SUITE_P(ThreadTest, LockFreeStackTest, ::testing::ValuesIn(threads_nums));
