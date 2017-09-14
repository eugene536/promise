//
// Created by eugene on 14.9.2017
//
#ifdef _GTEST

#include "thread_pool.h"

#include <atomic>

#include <gtest/gtest.h>

TEST(thread_pool, one_thread) {
    std::atomic<int> counter(0);

    {
        thread_pool pool(1);

        pool.execute([&counter]{ counter = 1; });
    }

    ASSERT_EQ(counter, 1);
}

TEST(thread_pool, ten_threads) {
    std::atomic<int> counter(0);

    int32_t const cnt_threads = 10;
    {
        thread_pool pool(cnt_threads);

        for (int32_t i = 0; i < cnt_threads; ++i) {
            pool.execute([&counter]{ counter += 1; });
        }
    }

    ASSERT_EQ(counter, cnt_threads);
}

#endif // _GTEST
