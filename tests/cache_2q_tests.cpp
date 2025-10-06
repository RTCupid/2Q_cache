#include <gtest/gtest.h>

#include "cache_2q.hpp"

int slow_get_elem (int key) { return key; }

static constexpr size_t capacity_in_tests = 20;

static constexpr float factor_size_list_in   = 0.2f;
static constexpr float factor_size_list_main = 0.2f;
static constexpr float factor_size_list_out  = 0.6f;

using namespace caches;

TEST (cache2q, overflow_queue_in)
{
    // arrange
    cache2q<int, int> cache(capacity_in_tests, slow_get_elem);

    // act
    for (int i = 0; i <= capacity_in_tests * factor_size_list_in; i++)
        cache.lookup_update (i);

    // assert
    for (int i = 1; i < capacity_in_tests * factor_size_list_in; i++)
        EXPECT_TRUE (cache.lookup_update (i));

    EXPECT_FALSE (cache.lookup_update (0));
}

struct test_t
{
    size_t cache_size;
    size_t number_elements;
    std::vector<int> elements;
    size_t cache_hits;
};

TEST (cache2q, end_to_end_tests)
{
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
        {
            10,
            11,
            {1, 2, 3, 4, 1, 2, 5, 6, 3, 4, 3},
            1
        },
        {
            8,
            10,
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
            0
        },
        {
            15,
            20,
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5},
            3
        },
        {
            10,
            10,
            {1, 2, 3, 4, 5, 6, 7, 8, 1, 1},
            1
        },
        {
            10,
            10,
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            9
        },
        {
            5,
            22,
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 1},
            1
        },
    };

    size_t number_tests = 8;

    for (size_t n_test = 0; n_test < number_tests; n_test++)
    {
        // arrange
        size_t cache_size = arr_tests[n_test].cache_size;

        cache2q<int, int> cache(cache_size, slow_get_elem);

        size_t number_elements = arr_tests[n_test].number_elements;

        size_t cache_hits = 0;

        // act
        for (int ind_elem = 0; ind_elem < number_elements; ind_elem++)
            cache_hits += cache.lookup_update (arr_tests[n_test].elements[ind_elem]);

        // assert
        EXPECT_EQ (cache_hits, arr_tests[n_test].cache_hits);
    }
}
