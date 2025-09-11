#include <iostream>
#include <cassert>
#include "../hdr/cache2q.hpp"

int the_slowest_function_to_get_int_element (int key) { return key; }

int main () {
    size_t cache_size = 0;

    std::cin >> cache_size;
    assert (std::cin.good());

    cache2q_t<page_t, int> cache2q(cache_size);

    size_t number_elems = 0;
    std::cin >> number_elems;

    page_t first_page = {100};

    cache2q.cache2q_lookup_update (number_elems, the_slowest_function_to_get_int_element);

    return 0;
}
