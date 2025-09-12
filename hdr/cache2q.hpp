#ifndef CACHE2Q_H
#define CACHE2Q_H

#include <iostream>
#include <list>
#include <unordered_map>

const float FACTOR_SIZE_LIST_IN   = 0.2;
const float FACTOR_SIZE_LIST_MAIN = 0.2;
const float FACTOR_SIZE_LIST_OUT  = 0.6;
//                            sum = 1

enum queue_t {
    IN,
    MAIN,
    OUT,
};

template <typename ElemT, typename KeyT>
class cache2q_t {
    size_t size_;

    std::list<ElemT> list_in_;
    std::list<ElemT> list_main_;
    std::list<KeyT>  list_out_;

    std::unordered_map<KeyT, std::pair<typename std::list<ElemT>::iterator, queue_t>> hash_table_in_and_main_;
    std::unordered_map<KeyT, typename std::list<KeyT>::iterator>  hash_table_out_;

public:
    cache2q_t (size_t size) : size_ (size) {
        list_in_.resize   (size * FACTOR_SIZE_LIST_IN);
        list_main_.resize (size * FACTOR_SIZE_LIST_MAIN);
        list_out_.resize  (size * FACTOR_SIZE_LIST_OUT);
    }

    template <typename FuncToGetElem_t>
    void cache2q_lookup_update (KeyT key, FuncToGetElem_t the_slowest_function_to_get_elem_from_key) {
        auto data_from_key = hash_table_in_and_main_.find (key);

        if (data_from_key != hash_table_in_and_main_.end ()) {
            if (hash_table_in_and_main_[key].second == queue_t::MAIN) {
                list_main_.splice (list_main_.begin (), list_main_, hash_table_in_and_main_[key].first);
                hash_table_in_and_main_[key] = std::make_pair (list_main_.begin (), queue_t::MAIN);
            }
        } else {
            auto data_from_key_out = hash_table_out_.find (key);

            if (data_from_key_out != hash_table_out_.end ()) {
                list_main_.push_front (the_slowest_function_to_get_elem_from_key (key));
                hash_table_in_and_main_[key] = std::make_pair (list_main_.begin (), queue_t::MAIN);

                hash_table_out_.erase (key);
                list_out_.erase (data_from_key_out->second);
            } else {
                list_in_.push_front (the_slowest_function_to_get_elem_from_key (key));
                hash_table_in_and_main_[key] = std::make_pair (list_main_.begin (), queue_t::IN);
            }
        }

    }
};

typedef int page_id_t;

struct page_t {
    page_id_t page_id;
};

#endif
