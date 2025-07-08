#include <gtest/gtest.h>
#include "counter_app.h"

class CounterTest {
public:
    int count = 0;
    
    [[nodiscard]] auto getCount() const -> int { return count; }
    auto increment() -> void { count++; }
    auto decrement() -> void { count--; }
    auto reset() -> void { count = 0; }
};

TEST(CounterAppTest, InitialCountIsZero) {
    CounterTest counter;
    EXPECT_EQ(counter.getCount(), 0);
}

TEST(CounterAppTest, IncrementCount) {
    CounterTest counter;
    counter.increment();
    EXPECT_EQ(counter.getCount(), 1);
    
    counter.increment();
    EXPECT_EQ(counter.getCount(), 2);
}

TEST(CounterAppTest, DecrementCount) {
    CounterTest counter;
    counter.increment();
    counter.increment();
    counter.decrement();
    EXPECT_EQ(counter.getCount(), 1);
}

TEST(CounterAppTest, ResetCount) {
    CounterTest counter;
    counter.increment();
    counter.increment();
    counter.increment();
    EXPECT_EQ(counter.getCount(), 3);
    
    counter.reset();
    EXPECT_EQ(counter.getCount(), 0);
}

TEST(CounterAppTest, DecrementBelowZero) {
    CounterTest counter;
    counter.decrement();
    EXPECT_EQ(counter.getCount(), -1);
}