#include <iostream>
#include <cassert>

#include "cache2q.hpp"
#include "colors.h"

int slow_func_to_get_int_element (int key) { return key; }

int main () {

    size_t cache_size = 0;

    std::cin >> cache_size;
    assert (std::cin.good());

    cache2q_t<int, int> cache2q(cache_size, slow_func_to_get_int_element);

    size_t number_elems = 0;

    std::cin >> number_elems;
    assert (std::cin.good());

    int page_id = 0;
    size_t hits = 0;

    for (size_t i = 0; i < number_elems; i++) {

        std::cin  >> page_id;
        assert (std::cin.good());

        if (cache2q.lookup_update (page_id))
            hits++;
    }

    std::cout << GRN "### number of hits = " YEL << hits << std::endl;

    return 0;
}
