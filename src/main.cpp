#include <iostream>
#include "../hdr/cache2q.hpp"

int main () {
    cache2q_t<page_t> cache2q;

    page_t first_page = {10};

    cache2q.cache2q_lookup_update (first_page);

    return 0;
}
