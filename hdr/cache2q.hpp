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

    std::list<ElemT> list_in_;
    std::list<ElemT> list_main_;
    std::list<KeyT>  list_out_;

    std::unordered_map<KeyT, std::pair<typename std::list<ElemT>::iterator, queue_t>> hash_table_in_and_main_;
    std::unordered_map<KeyT, typename std::list<KeyT>::iterator>  hash_table_out_;

public:
    cache2q_t (size_t size) {

        size_list_in_ = size * FACTOR_SIZE_LIST_IN;
        list_in_.resize   (size_list_in_);

        size_list_main_ = size * FACTOR_SIZE_LIST_MAIN;
        list_main_.resize (size_list_main_);

        size_list_out_ = size * FACTOR_SIZE_LIST_OUT;
        list_out_.resize  (size_list_out_);
    }

    template <typename FuncToGetElem_t>
    bool cache2q_lookup_update (KeyT key, FuncToGetElem_t the_slowest_function_to_get_elem_from_key) {

        auto data_from_key = hash_table_in_and_main_.find (key);

        if (data_from_key != hash_table_in_and_main_.end ()) {

            if (hash_table_in_and_main_[key].second == queue_t::MAIN) {

                list_main_.splice (list_main_.begin (), list_main_, hash_table_in_and_main_[key].first);
                hash_table_in_and_main_[key] = std::make_pair (list_main_.begin (), queue_t::MAIN);
            }

            else {

                if (list_in_.size () >= size_list_in_) {

                    
                }
            }

            return true;
        }

        auto data_from_key_out = hash_table_out_.find (key);

        if (data_from_key_out != hash_table_out_.end ()) {

            insert_to_main (key, the_slowest_function_to_get_elem_from_key (key));

            erase_from_out (key, data_from_key_out->second);
        }

        else {

            insert_to_in (key, the_slowest_function_to_get_elem_from_key (key));
        }
        return false;
    }
private:
    size_t size_list_in_   = 0;
    size_t size_list_main_ = 0;
    size_t size_list_out_  = 0;

    void insert_to_in (KeyT key, ElemT new_elem_to_in) {

        list_in_.push_front (new_elem_to_in);
        hash_table_in_and_main_[key] = std::make_pair (list_in_.begin (), queue_t::IN);
    }

    void insert_to_main (KeyT key, ElemT new_elem_to_main) {

        list_main_.push_front (new_elem_to_main);
        hash_table_in_and_main_[key] = std::make_pair (list_main_.begin (), queue_t::MAIN);
    }

    void erase_from_out (KeyT key, typename std::list<KeyT>::iterator iterator_list_out) {

        hash_table_out_.erase (key);
        list_out_.erase (iterator_list_out);
    }

};

typedef int page_t;

#endif
