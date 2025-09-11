#ifndef CACHE2Q_H
#define CACHE2Q_H

typedef int page_id_t;

template <typename T>
class cache2q_t {
    size_t size_;

public:
    void cache2q_lookup_update (T page);
};

template <typename T>
void cache2q_t<T>::cache2q_lookup_update (T page) {
    std::cout << "Hello, world!\n";
}

struct page_t {
    page_id_t page_id;
};

#endif
