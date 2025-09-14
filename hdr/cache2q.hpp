#ifndef CACHE2Q_H
#define CACHE2Q_H

#include <iostream>
#include <list>
#include <unordered_map>

#include "../common/colors.h"
#include "../common/config.h"

const float FACTOR_SIZE_LIST_IN   = 0.2;
const float FACTOR_SIZE_LIST_MAIN = 0.2;
const float FACTOR_SIZE_LIST_OUT  = 0.6;
//                            sum = 1

const size_t MINIMAL_CAPACITY = 5;

enum queue_t {

    IN,
    MAIN,
    OUT,
};

template <typename ElemT, typename KeyT>
class cache2q_t {

    std::list<std::pair<KeyT, ElemT>> list_in_;
    std::list<std::pair<KeyT, ElemT>> list_main_;
    std::list<KeyT>  list_out_;

    std::unordered_map<KeyT, typename std::list<std::pair<KeyT, ElemT>>::iterator> hash_table_in_;
    std::unordered_map<KeyT, typename std::list<std::pair<KeyT, ElemT>>::iterator> hash_table_main_;
    std::unordered_map<KeyT, typename std::list<KeyT>::iterator>                   hash_table_out_;

public:
    cache2q_t (size_t size) {

        size = std::max (size, MINIMAL_CAPACITY);

        DEBUG_PRINT("all size: %lu\n", size);

        size_list_in_ = size * FACTOR_SIZE_LIST_IN;

        DEBUG_PRINT("list in size: %lu\n", size_list_in_);

        size_list_main_ = size * FACTOR_SIZE_LIST_MAIN;

        DEBUG_PRINT("list main size: %lu\n", size_list_main_);

        size_list_out_ = size * FACTOR_SIZE_LIST_OUT;

        DEBUG_PRINT("list out size: %lu\n", size_list_out_);
    }

    template <typename FuncToGetElem_t>
    bool cache2q_lookup_update (KeyT key, FuncToGetElem_t the_slowest_function_to_get_elem_from_key) {

        DEBUG_PRINT("Start search:\n");

        auto data_from_key_in = hash_table_in_.find (key);

        if (data_from_key_in != hash_table_in_.end ()) {

            DEBUG_PRINT("It is in queue in\n");

            return true;
        }

        auto data_from_key_main = hash_table_main_.find (key);

        if (data_from_key_main != hash_table_main_.end ()) {

            DEBUG_PRINT("It is in queue main\n");

            list_main_.splice (list_main_.begin (), list_main_, hash_table_main_[key]);
            hash_table_main_[key] = list_main_.begin ();

            return true;
        }

        auto data_from_key_out = hash_table_out_.find (key);

        if (data_from_key_out != hash_table_out_.end ()) {

            DEBUG_PRINT("It is in queue out\n");

            insert_to_main (key, the_slowest_function_to_get_elem_from_key (key));

            erase_from_out (data_from_key_out->second);

            return false;
        }

        insert_to_in (key, the_slowest_function_to_get_elem_from_key (key));

        DEBUG_PRINT("Not found and inserted to queue in\n");

        return false;
    }

    void dump_queues () const {

        std::cout << GRN "start -> end" RESET << std::endl;

        std::cout << MAG "list in:" CYN << std::endl;

        for (auto &it : list_in_) {

            std::cout << it.second << " ";
        }

        std::cout << std::endl;

        std::cout << MAG "list main:" CYN << std::endl;

        for (auto &it : list_main_) {

            std::cout << it.second << " ";
        }

        std::cout << std::endl;

        std::cout << MAG "list out:" CYN << std::endl;

        for (auto &it : list_out_) {

            std::cout << it << " ";
        }

        std::cout << std::endl;
    }

private:
    size_t size_list_in_   = 0;
    size_t size_list_main_ = 0;
    size_t size_list_out_  = 0;

    void insert_to_in (KeyT key, ElemT new_elem_to_in) {

        if (list_in_.size () >= size_list_in_) {

            insert_to_out (list_in_.back ().first);

            erase_last_elem_from_in ();
        }

        list_in_.push_front (std::make_pair(key, new_elem_to_in));
        hash_table_in_[key] = list_in_.begin ();
    }

    void insert_to_main (KeyT key, ElemT new_elem_to_main) {

        if (list_main_.size () >= size_list_main_)
            erase_last_elem_from_main ();

        list_main_.push_front (std::make_pair(key, new_elem_to_main));
        hash_table_main_[key] = list_main_.begin ();
    }

    void insert_to_out (KeyT key) {

        if (list_out_.size () >= size_list_out_)
            list_out_.pop_back ();

        list_out_.push_front (key);

        hash_table_out_[key] = list_out_.begin ();
    }

    void erase_from_out (typename std::list<KeyT>::iterator iterator_list_out) {

        hash_table_out_.erase (*iterator_list_out);
        list_out_.erase (iterator_list_out);
    }

    void erase_last_elem_from_in () {

        hash_table_in_.erase (list_in_.back ().first);

        list_in_.pop_back ();
    }

    void erase_last_elem_from_main () {

        hash_table_main_.erase (list_main_.back ().first);

        list_main_.pop_back ();
    }
};

typedef int page_t;

#endif
