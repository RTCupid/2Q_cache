#ifndef CACHE2Q_H
#define CACHE2Q_H

#include <iostream>
#include <list>
#include <unordered_map>

const float FACTOR_SIZE_LIST_IN   = 0.2;
const float FACTOR_SIZE_LIST_MAIN = 0.2;
const float FACTOR_SIZE_LIST_OUT  = 0.6;
//                            sum = 1

template <typename ElemT, typename KeyT>
class cache2q_t {
    size_t size_;

    std::list<ElemT> list_in_;
    std::list<ElemT> list_main_;
    std::list<KeyT>  list_out_;

    std::unordered_map<KeyT, typename std::list<ElemT>::iterator> hash_table_in_;
    std::unordered_map<KeyT, typename std::list<ElemT>::iterator> hash_table_main_;
    std::unordered_map<KeyT, typename std::list<KeyT>::iterator>  hash_table_out_;

public:
    cache2q_t (size_t size) : size_ (size) {
        list_in_.resize   (size * FACTOR_SIZE_LIST_IN);
        list_main_.resize (size * FACTOR_SIZE_LIST_MAIN);
        list_out_.resize  (size * FACTOR_SIZE_LIST_OUT);
    }

    template <typename FuncToGetElem_t>
    void cache2q_lookup_update (KeyT key, FuncToGetElem_t the_slowest_function_to_get_elem_from_key) {
        if (auto iter_key = hash_table_in_.find (key) != hash_table_in_.end ()) {
            std::cout << "find key in hash_table_in. Continue \n";

        } else if ((auto iter_key = hash_table_main_.find (key)) != hash_table_main_.end ()) {
            std::cout << "find key in hash_table_main. Process LRU algorithm...\n";

            list_main_.splice (list_main_.begin (), list_main_, iter_key);
            hash_table_main_[key] = list_main_.begin ();

        } else if (auto iter_key = hash_table_out_.find (key) != hash_table_out_.end ()) {
            std::cout << "find key in hash_table_out. Process load to main list... \n";

            list_main_.push_front (the_slowest_function_to_get_elem_from_key (key));
            hash_table_main_[key] = list_main_.begin ();

            hash_table_out_.erase (key);
            list_out_.erase (iter_key);
        } else {
            std::cout << "not found. Process load to list in \n";

            list_in_.push_front (the_slowest_function_to_get_elem_from_key (key));
            hash_table_in_[key] = list_in_.begin ();
        }

    }
};

typedef int page_id_t;

struct page_t {
    page_id_t page_id;
};

#endif
