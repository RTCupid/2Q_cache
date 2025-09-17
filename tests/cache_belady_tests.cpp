#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include "cache_belady.hpp"

int the_slowest_function_to_get_int_element (int key) { return key; }

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
            3,
            12,
            {1, 2, 3, 4, 1, 3, 5, 6, 8, 7, 2, 1},
            3
        }
    };

    size_t number_tests = 2;

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

        belady_cache<int, int> cache(cache_size, the_slowest_function_to_get_int_element, vector_elements);

        int cache_hits = 0;

        // act
        for (size_t ind_elem = 0; ind_elem < actual_size; ind_elem++)
            cache_hits += cache.lookup_update (vector_elements[ind_elem]);

        // assert
        EXPECT_EQ (cache_hits, arr_tests[n_test].cache_hits);
    }
}
