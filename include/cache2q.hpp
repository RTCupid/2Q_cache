#ifndef CACHE2Q_H
#define CACHE2Q_H

#include <functional>
#include <iostream>
#include <list>
#include <unordered_map>

#include "colors.h"

template <typename KeyT, typename ElemT>
class cache2q {

    static constexpr float factor_size_list_in_   = 0.2f;
    static constexpr float factor_size_list_main_ = 0.2f;
    static constexpr float factor_size_list_out_  = 0.6f;

    static_assert (factor_size_list_in_ + factor_size_list_main_ + factor_size_list_out_ == 1.0f, "Sum of factors must be 1.0f\n");

    static constexpr size_t minimal_capacity_ = 5;

    using ElemListT = std::list<std::pair<KeyT, ElemT>>;
    using ElemIterT = ElemListT::iterator;

    using KeyListT  = std::list<KeyT>;
    using KeyIterT  = KeyListT::iterator;

    using FuncToGetElem = std::function<ElemT(KeyT)>;

    ElemListT list_in_;
    ElemListT list_main_;
    KeyListT  list_out_;

    std::unordered_map<KeyT, ElemIterT> hash_table_in_;
    std::unordered_map<KeyT, ElemIterT> hash_table_main_;
    std::unordered_map<KeyT, KeyIterT>  hash_table_out_;

    FuncToGetElem slow_get_elem_;

    const size_t size_;
    const size_t size_list_in_;
    const size_t size_list_main_;
    const size_t size_list_out_;

public:
    cache2q (size_t size, FuncToGetElem slow_get_elem) :

        size_           (std::max (size, minimal_capacity_)),

        size_list_in_   (size_ * factor_size_list_in_),

        size_list_main_ (size_ * factor_size_list_main_),

        size_list_out_  (size_ * factor_size_list_out_),

        slow_get_elem_  (slow_get_elem) {
    }

    bool lookup_update (KeyT key) {

        auto data_from_key_in = hash_table_in_.find (key);

        if (data_from_key_in != hash_table_in_.end ()) {

            return true;
        }

        auto data_from_key_main = hash_table_main_.find (key);

        if (data_from_key_main != hash_table_main_.end ()) {

            list_main_.splice (list_main_.begin (), list_main_, data_from_key_main->second);
            data_from_key_main->second = list_main_.begin ();

            return true;
        }

        auto new_elem = slow_get_elem_ (key);

        auto data_from_key_out = hash_table_out_.find (key);

        if (data_from_key_out != hash_table_out_.end ()) {

            insert_to_main (key, new_elem);

            erase_from_out (data_from_key_out->second);

            return false;
        }

        insert_to_in (key, new_elem);

        return false;
    }

    void dump_queues () const {

        std::cout << GRN "start -> end\n" RESET;

        std::cout << MAG "list in:\n" CYN;

        for (auto &it : list_in_)
            std::cout << it.second << ' ';

        std::cout << '\n';

        std::cout << MAG "list main:\n" CYN;

        for (auto &it : list_main_)
            std::cout << it.second << ' ';

        std::cout << '\n';

        std::cout << MAG "list out:\n" CYN;

        for (auto &it : list_out_)
            std::cout << it << ' ';

        std::cout << '\n';
    }

private:

    void insert_to_in (KeyT& key, ElemT& new_elem_to_in) {

        if (list_in_.size () >= size_list_in_) {

            insert_to_out (list_in_.back ().first);

            erase_last_elem_from_in ();
        }

        list_in_.push_front (std::make_pair(key, new_elem_to_in));
        hash_table_in_[key] = list_in_.begin ();
    }

    void insert_to_main (KeyT& key, ElemT& new_elem_to_main) {

        if (list_main_.size () >= size_list_main_)
            erase_last_elem_from_main ();

        list_main_.push_front (std::make_pair(key, new_elem_to_main));
        hash_table_main_[key] = list_main_.begin ();
    }

    void insert_to_out (KeyT &key) {

        if (list_out_.size () >= size_list_out_)
            list_out_.pop_back ();

        list_out_.push_front (key);

        hash_table_out_[key] = list_out_.begin ();
    }

    void erase_from_out (KeyIterT& iterator_list_out) {

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

#endif
