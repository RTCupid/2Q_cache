#ifndef CACHE2Q_H
#define CACHE2Q_H

#include <functional>
#include <iostream>
#include <list>
#include <unordered_map>

namespace detail
{

template <typename KeyT, typename ElemT>
class queueLRU
{
    using ElemListT     = std::list<std::pair<KeyT, ElemT>>;
    using ElemIterT     = ElemListT::iterator;
    using MapElemIter   = std::unordered_map<KeyT, ElemIterT>;
    using FuncToGetElem = std::function<ElemT(const KeyT &)>;

    size_t size_;
    ElemListT list_;
    MapElemIter hash_table_;
    FuncToGetElem slow_get_elem_;

public:
    queueLRU (size_t size, FuncToGetElem slow_get_elem) :
        size_ (size), slow_get_elem_  (slow_get_elem) {}

    bool lookup_update (const KeyT &key)
    {
        auto data_from_key_main = hash_table_.find (key);

        if (data_from_key_main != hash_table_.end ())
        {
            list_.splice (list_.begin (), list_, data_from_key_main->second);
            data_from_key_main->second = list_.begin ();

            return true;
        }

        return false;
    }

    bool insert (const KeyT &key, const ElemT &new_elem_to_main)
    {
        auto data_from_key_main = hash_table_.find (key);

        if (data_from_key_main != hash_table_.end ())
            return false;

        if (list_.size () == size_)
        {
            hash_table_.erase (list_.back ().first);
            list_.pop_back ();
        }

        list_.emplace_front (key, new_elem_to_main);
        hash_table_.emplace (key, list_.begin ());

        return true;
    }

    void dump_list ()
    {
        for (auto &elem : list_)
            std::cout << elem.second << ' ';

        std::cout << '\n';
    }

    void dump_hashtable ()
    {
        for (auto &elem_iter : hash_table_)
            std::cout << elem_iter.first << ' ';

        std::cout << '\n';
    }
};

} // namespace detail

namespace caches
{

template <typename KeyT, typename ElemT>
class cache2q
{
    static constexpr float factor_size_list_in_   = 0.2f;
    static constexpr float factor_size_list_main_ = 0.2f;
    static constexpr float factor_size_list_out_  = 0.6f;

    static constexpr size_t minimal_capacity_ = 5;

    using ElemListT = std::list<std::pair<KeyT, ElemT>>;
    using ElemIterT = ElemListT::iterator;

    using KeyListT  = std::list<KeyT>;
    using KeyIterT  = KeyListT::iterator;

    using FuncToGetElem = std::function<ElemT(const KeyT &)>;

    size_t size_;
    size_t size_list_in_;
    size_t size_list_main_;
    size_t size_list_out_;

    FuncToGetElem slow_get_elem_;

    ElemListT list_in_;
    KeyListT  list_out_;

    std::unordered_map<KeyT, ElemIterT> hash_table_in_;
    std::unordered_map<KeyT, KeyIterT>  hash_table_out_;

    detail::queueLRU<KeyT, ElemT> queue_main_;

public:
    cache2q (size_t size, FuncToGetElem slow_get_elem) :
        size_           (std::max (size, minimal_capacity_)),
        size_list_in_   (size_ * factor_size_list_in_),
        size_list_main_ (size_ * factor_size_list_main_),
        size_list_out_  (size_ * factor_size_list_out_),
        slow_get_elem_  (slow_get_elem),
        queue_main_     (size_list_main_, slow_get_elem) {}

    bool lookup_update (const KeyT &key)
    {
        if (hash_table_in_.contains (key) )
            return true;

        if (queue_main_.lookup_update(key))
            return true;

        auto new_elem = slow_get_elem_ (key);

        auto data_from_key_out = hash_table_out_.find (key);

        if (data_from_key_out == hash_table_out_.end ())
        {
            insert_to_in (key, new_elem);
        }
        else
        {
            queue_main_.insert (key, new_elem);

            list_out_.erase (data_from_key_out->second);
            hash_table_out_.erase (data_from_key_out);
        }

        return false;
    }

    void dump_queues () const
    {
        std::cout << "start -> end\n";

        std::cout << "list in:\n";

        for (auto &elem : list_in_)
            std::cout << elem.second << ' ';

        std::cout << '\n';

        std::cout << "list main:\n";

        queue_main_.dump_list ();

        std::cout << "list out:\n";

        for (auto &key : list_out_)
            std::cout << key << ' ';

        std::cout << '\n';
    }

    void dump_hash_tables () const
    {
        std::cout << "start -> end\n";

        std::cout << "hash_table in:\n";

        for (auto &elem_iter : hash_table_in_)
            std::cout << elem_iter.first << ' ';

        std::cout << '\n';

        std::cout << "hash_table main:\n";

        queue_main_.dump_hashtable ();

        std::cout << "hash_table out:\n";

        for (auto &key_iter : hash_table_out_)
            std::cout << key_iter.first << ' ';

        std::cout << '\n';
    }

private:

    void insert_to_in (const KeyT &key, const ElemT &new_elem_to_in)
    {
        if (list_in_.size () == size_list_in_)
        {
            insert_to_out (list_in_.back ().first);

            hash_table_in_.erase (list_in_.back ().first);
            list_in_.pop_back ();
        }

        list_in_.emplace_front (key, new_elem_to_in);
        hash_table_in_.emplace (key, list_in_.begin ());
    }

    void insert_to_out (const KeyT &key)
    {
        if (list_out_.size () == size_list_out_)
        {
            hash_table_out_.erase (list_out_.back ());
            list_out_.pop_back ();
        }

        list_out_.emplace_front (key);
        hash_table_out_.emplace (key, list_out_.begin ());
    }
};
} // namespace caches

#endif
