#include <iostream>
#include <cassert>

#include "cache_2q.hpp"

int slow_func_to_get_int_element (int key) { return key; }

using namespace caches;

int main ()
{
    size_t cache_size;

    std::cin >> cache_size;
    if (!std::cin.good ())
    {
        std::cerr << "error reading cache size\n";
        return EXIT_FAILURE;
    }

    cache2q<int, int> cache (cache_size, slow_func_to_get_int_element);

    size_t number_elems;

    std::cin >> number_elems;
    if (!std::cin.good ())
    {
        std::cerr << "error reading number elements\n";
        return EXIT_FAILURE;
    }

    int page_id;
    size_t hits = 0;

    for (size_t i = 0; i < number_elems; i++)
    {
        std::cin  >> page_id;
        if (!std::cin.good ())
        {
            std::cerr << "error reading page_id " << i << '\n';
            return EXIT_FAILURE;
        }

        hits += cache.lookup_update (page_id);
    }

    std::cout << hits << '\n';

    return 0;
}
