#ifndef CACHE2Q_H
#define CACHE2Q_H

#include <iostream>
#include <list>
#include <unordered_map>

const float FACTOR_SIZE_LIST_IN   = 0.2;
const float FACTOR_SIZE_LIST_MAIN = 0.2;
const float FACTOR_SIZE_LIST_OUT  = 0.6;
//                        sum = 1

template <typename ElemT, typename KeyT>
class cache2q_t {
    size_t size_;

    std::list<ElemT> list_in_;
    std::list<ElemT> list_main_;
    std::list<KeyT>  list_out_;

    std::unordered_map<KeyT, typename std::list<ElemT>::iterator> hash_table_in;
    std::unordered_map<KeyT, typename std::list<ElemT>::iterator> hash_table_main;
    std::unordered_map<KeyT, typename std::list<KeyT>::iterator>  hash_table_out;

public:
    cache2q_t (size_t size) : size_(size) {
        list_in_.resize   (size * FACTOR_SIZE_LIST_IN);
        list_main_.resize (size * FACTOR_SIZE_LIST_MAIN);
        list_out_.resize  (size * FACTOR_SIZE_LIST_OUT);
    }

    template <typename FuncToGetElem_t>
    void cache2q_lookup_update (KeyT key, FuncToGetElem_t the_slowest_function_to_get_elems) {
        if (hash_table_in.find(key) != hash_table_in.end()) {
            std::cout << "find key in hash_table_in  \n";
        } else if (hash_table_main.find(key) != hash_table_main.end()) {
            std::cout << "find key in hash_table_main\n";
        } else if (hash_table_out.find(key) != hash_table_out.end()) {
            std::cout << "find key in hash_table_out \n";
        } else {
            std::cout << "not found \n";
        }

    }
};

typedef int page_id_t;

struct page_t {
    page_id_t page_id;
};

#endif
