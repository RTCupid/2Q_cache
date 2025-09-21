#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include "cache_belady.hpp"

int slow_get_elem (int key) { return key; }

struct test_t
{
    size_t cache_size;
    size_t number_elements;
    std::vector<int> elements;
    size_t cache_hits;
};

TEST (cache_belady, end_to_end_tests)
{
    // arrange
    test_t arr_tests[] = {
        {
            3,
            10,
            {1, 2, 3, 4, 1, 6, 3, 6, 3, 7},
            4
        },
        {
            10,
            25,
            {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
            13
        },
        {
            10,
            30,
            {5, 12, 5, 18, 12, 5, 3, 18, 12, 5, 3, 18, 12, 5, 3, 18, 12, 5, 3, 18, 12, 5, 3, 18, 12, 5, 3, 18, 5, 3},
            26
        },
        {
            15,
            35,
            {1, 6, 11, 16, 21, 1, 6, 11, 16, 21, 26, 31, 1, 6, 11, 16, 21, 26, 31, 1, 6, 11, 16, 21, 26, 31, 1, 6, 11, 26, 21, 31, 6, 11, 1},
            28
        },
        {
            10,
            28,
            {3, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9, 8, 10, 9, 11, 10, 12, 11, 13, 12, 14, 13, 15, 14, 16, 13, 17},
            13
        },
        {
            15,
            40,
            {2, 5, 8, 2, 5, 8, 11, 14, 2, 5, 8, 11, 14, 17, 20, 2, 5, 8, 11, 14, 17, 20, 23, 26, 2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 2, 5, 8, 11, 14},
            29
        },
        {
            3,
            11,
            {1, 2, 3, 4, 1, 1, 2, 2, 3, 3, 4},
            6
        }
    };

    size_t number_tests = 7;

    for (size_t n_test = 0; n_test < number_tests; n_test++)
    {
        size_t cache_size = arr_tests[n_test].cache_size;

        std::vector<int> vector_elements;

        size_t number_elems = arr_tests[n_test].number_elements;
        const auto& elements = arr_tests[n_test].elements;

        size_t actual_size = std::min(number_elems, elements.size());

        for (size_t i = 0; i < actual_size; i++)
        {
            vector_elements.emplace_back (elements[i]);
        }

        belady_cache<int, int> cache(cache_size, slow_get_elem, vector_elements);

        int cache_hits = 0;

        // act
        for (size_t ind_elem = 0; ind_elem < actual_size; ind_elem++)
            cache_hits += cache.lookup_update (vector_elements[ind_elem]);

        // assert
        EXPECT_EQ (cache_hits, arr_tests[n_test].cache_hits);
    }
}
