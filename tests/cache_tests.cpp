#include <gtest/gtest.h>
#include <iostream>

#include "cache2q.hpp"

int the_slowest_function_to_get_int_element (int key) { return key; }

static const size_t CAPACITY_IN_TESTS = 20;

static constexpr float FACTOR_SIZE_LIST_IN   = 0.2f;
static constexpr float FACTOR_SIZE_LIST_MAIN = 0.2f;
static constexpr float FACTOR_SIZE_LIST_OUT  = 0.6f;

TEST (cache2q, overflow_queue_in) {

    // arrange
    size_t cache_size = CAPACITY_IN_TESTS;

    cache2q_t<int, int> cache2q(cache_size, the_slowest_function_to_get_int_element);

    // act
    for (int i = 0; i <= cache_size * FACTOR_SIZE_LIST_IN; i++) {
        cache2q.lookup_update (i);
    }
    // there queue in is full and include 1...(cache_size * FACTOR_SIZE_LIST_IN - 1), and not include 0

    // assert
    for (int i = 1; i < cache_size * FACTOR_SIZE_LIST_IN; i++) {
        EXPECT_TRUE (cache2q.lookup_update (i));
    }

    EXPECT_FALSE (cache2q.lookup_update (0));
}

TEST (cache2q, overflow_queue_main) {

    // arrange
    size_t cache_size = CAPACITY_IN_TESTS;

    cache2q_t<int, int> cache2q(cache_size, the_slowest_function_to_get_int_element);

    auto size_list_in   = cache_size * FACTOR_SIZE_LIST_IN;
    auto size_list_main = cache_size * FACTOR_SIZE_LIST_MAIN;
    auto num_different_elements = size_list_in + size_list_main;

    // act
    for (int i = 0; i < size_list_in; i++) {
        cache2q.lookup_update (i);
    }
    // there queue in is full

    for (int i = size_list_in; i <= num_different_elements; i++) {
        cache2q.lookup_update (i);
        cache2q.lookup_update (i - size_list_in);
    }
    // there queue in is full and include num_different_elements...num_different_elements - size_list_in,
    // queue out is empty, queue main is full and include num_different_elements - size_list_in...1

    // assert
    for (int i = 1; i <= num_different_elements; i++) {
        EXPECT_TRUE (cache2q.lookup_update (i));
    }

    EXPECT_FALSE (cache2q.lookup_update (0));
}
