#include <iostream>
#include <cassert>

#include "../hdr/cache2q.hpp"
#include "../colors.h"

page_t the_slowest_function_to_get_int_element (int key) { return key; }

int main () {
    std::cout << GRN "### realization of 2Q cache\n" RESET;
    std::cout << GRN "# (c) RTCupid, 2024\n\n" RESET;

    size_t cache_size = 0;

    std::cout << BLU "Enter cache size: " YEL;
    std::cin >> cache_size;
    assert (std::cin.good());

    cache2q_t<page_t, int> cache2q(cache_size);

    size_t number_elems = 0;

    std::cout << BLU "Enter number of elements: " YEL;
    std::cin >> number_elems;

    page_t page_id = 0;
    size_t hits    = 0;

    for (size_t i = 0; i < number_elems; i++) {
        std::cout << BLU "Enter " << i << " element: " YEL;
        std::cin  >> page_id;

        if (cache2q.cache2q_lookup_update (page_id, the_slowest_function_to_get_int_element)) {
            std::cout << GRN "cache hit " YEL << page_id << std::endl;
            hits++;
        }
    }

    std::cout << GRN "### number of hits = " YEL << hits << std::endl;

    return 0;
}
