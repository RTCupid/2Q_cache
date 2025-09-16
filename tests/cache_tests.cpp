#include <gtest/gtest.h>
#include <iostream>

#include "cache2q.hpp"

int the_slowest_function_to_get_int_element (int key) { return key; }
int run_cache_test (cache2q<int, int> cache);

static const size_t CAPACITY_IN_TESTS = 20;

static constexpr float FACTOR_SIZE_LIST_IN   = 0.2f;
static constexpr float FACTOR_SIZE_LIST_MAIN = 0.2f;
static constexpr float FACTOR_SIZE_LIST_OUT  = 0.6f;

TEST (cache2q, overflow_queue_in)
{
    // arrange
    size_t cache_size = CAPACITY_IN_TESTS;

    cache2q<int, int> cache(cache_size, the_slowest_function_to_get_int_element);

    // act
    for (int i = 0; i <= cache_size * FACTOR_SIZE_LIST_IN; i++)
        cache.lookup_update (i);
    // there queue in is full and include 1...(cache_size * FACTOR_SIZE_LIST_IN - 1), and not include 0

    // assert
    for (int i = 1; i < cache_size * FACTOR_SIZE_LIST_IN; i++)
        EXPECT_TRUE (cache.lookup_update (i));

    EXPECT_FALSE (cache.lookup_update (0));
}

TEST (cache2q, overflow_queue_main)
{
    // arrange
    size_t cache_size = CAPACITY_IN_TESTS;

    cache2q<int, int> cache(cache_size, the_slowest_function_to_get_int_element);

    auto size_list_in   = cache_size * FACTOR_SIZE_LIST_IN;
    auto size_list_main = cache_size * FACTOR_SIZE_LIST_MAIN;
    auto num_different_elements = size_list_in + size_list_main;

    // act
    for (int i = 0; i < size_list_in; i++)
        cache.lookup_update (i);
    // there queue in is full

    for (int i = size_list_in; i <= num_different_elements; i++)
    {
        cache.lookup_update (i);
        cache.lookup_update (i - size_list_in);
    }
    // there queue in is full and include num_different_elements...num_different_elements - size_list_in,
    // queue out is empty, queue main is full and include num_different_elements - size_list_in...1

    // assert
    for (int i = 1; i <= num_different_elements; i++)
    {
        EXPECT_TRUE (cache.lookup_update (i));
    }

    EXPECT_FALSE (cache.lookup_update (0));
}

struct test_t
{
    size_t cache_size;
    size_t number_elements;
    std::vector<int> elements;
    size_t cache_hits;
};

TEST (cache2q, end_to_end_tests) {

    test_t arr_tests[] = {
        {
            5,
            10,
            {1, 2, 2, 4, 1, 5, 1, 6, 3, 7},
            2
        },
        {
            10,
            5,
            {1, 2, 3, 1, 1},
            1
        },
    };

    size_t number_tests = 2;

    // arrange
    for (size_t n_test = 0; n_test < number_tests; n_test++)
    {
        size_t cache_size = arr_tests[n_test].cache_size;

        cache2q<int, int> cache(cache_size, the_slowest_function_to_get_int_element);

        size_t number_elements = arr_tests[n_test].number_elements;

        size_t cache_hits = 0;

        for (int ind_elem = 0; ind_elem < number_elements; ind_elem++)
            cache_hits += cache.lookup_update (arr_tests[n_test].elements[ind_elem]);

        EXPECT_EQ (cache_hits, arr_tests[n_test].cache_hits);
    }
}
