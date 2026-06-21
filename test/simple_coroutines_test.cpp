#include "simple.h"

#include <cstddef>
#include <gtest/gtest.h>

kat_coro::Generator<std::size_t> fibonacci() {
  std::size_t a = 0;
  std::size_t b = 1;

  for (;;) {
    a = std::exchange(b, a + b);
    co_yield b;
  }
}

TEST(GeneratorTest, FibonacciTest) {
  auto fib_num = fibonacci();
  std::size_t a = 0;
  std::size_t b = 1;

  for (std::size_t i = 0; i < 15; ++i) {
    EXPECT_EQ(fib_num.value(), a + b);
    a = std::exchange(b, a + b);
    fib_num.next();
  }
}
