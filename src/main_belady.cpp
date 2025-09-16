#include <iostream>
#include <cassert>

#include "cache_belady.hpp"
#include "colors.h"

int slow_func_to_get_int_element (int key) { return key; }

int main ()
{
    size_t cache_size;

    std::cin >> cache_size;
    if (!std::cin.good())
    {
        std::cerr << "error reading cache size\n";
        return EXIT_FAILURE;
    }

    size_t number_elems;

    std::cin >> number_elems;
    if (!std::cin.good())
    {
        std::cerr << "error reading number elements\n";
        return EXIT_FAILURE;
    }

    int page_id;
    std::vector<int> vector_elements;

    for (size_t i = 0; i < number_elems; i++)
    {
        std::cin  >> page_id;
        if (!std::cin.good())
        {
            std::cerr << "error reading page_id " << i << '\n';
            return EXIT_FAILURE;
        }

        vector_elements.emplace_back (page_id);
    }

    belady_cache<int, int> cache (cache_size, slow_func_to_get_int_element, vector_elements);

    size_t hits = 0;

    for (size_t i = 0; i < number_elems; i++)
    {
        page_id = vector_elements.back ();

        hits += cache.lookup_update (page_id);

        vector_elements.pop_back ();
    }

    std::cout << hits << '\n';

    return 0;
}
