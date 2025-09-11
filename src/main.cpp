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

    page_id_t page_id = 0;

    for (size_t i = 0; i < number_elems; i++) {
        std::cin >> page_id;

        cache2q.cache2q_lookup_update (page_id, the_slowest_function_to_get_int_element);
    }

    return 0;
}
