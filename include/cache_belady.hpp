#ifndef CACHE_BELADY_H
#define CACHE_BELADY_H

#include <iostream>
#include <vector>
#include <functional>
#include <queue>

namespace caches {

template <typename KeyT, typename ElemT>
class belady_cache
{
    using KeyVectorT    = std::vector<KeyT>;
    using FuncToGetElem = std::function<ElemT(const KeyT &)>;
    using CacheIterT    = std::pair<KeyT, ElemT>;

    size_t size_;
    size_t current_index_;
    FuncToGetElem slow_get_elem_;
    std::unordered_map<KeyT, ElemT> cache_;
    std::unordered_map<KeyT, std::queue<size_t>> key_positions_;

public:
    belady_cache (size_t size, FuncToGetElem slow_get_elem, KeyVectorT &input_elements) :
        size_ (size),
        current_index_ (0),
        slow_get_elem_ (slow_get_elem)
    {
        for (size_t i = 0; i < input_elements.size (); ++i)
            key_positions_[input_elements[i]].push (i);
    }

    bool lookup_update (const KeyT &key)
    {
        ++current_index_;
        auto &key_deque = key_positions_[key];
        key_deque.pop ();

        if (cache_.contains (key))
            return true;

        if (key_deque.empty ())
            return false;

        if (cache_.size () == size_)
        {
            if (erase_elem_from_belady_cache (key))
                cache_.emplace (key, slow_get_elem_ (key));

            return false;
        }

        cache_.emplace (key, slow_get_elem_ (key));

        return false;
    }

    void dump () const
    {
        std::cout << "Dump:\n";

        for (const auto &elem : cache_)
            std::cout << elem.second << ' ';

        std::cout << '\n';
    }

private:
    bool erase_elem_from_belady_cache (const KeyT &key_input_elem)
    {
        auto iter_elem_to_erase = cache_.end ();
        size_t max_distance     = key_positions_[key_input_elem].front () - current_index_;
        bool found_erase_elem   = false;

        for (auto elem_iter = cache_.begin(); elem_iter != cache_.end(); ++elem_iter)
        {
            auto &key_deque = key_positions_[elem_iter->first];

            if (key_deque.empty ())
            {
                iter_elem_to_erase = elem_iter;
                found_erase_elem   = true;
                break;
            }

            size_t distance = key_deque.front () - current_index_;

            if (distance > max_distance)
            {
                max_distance       = distance;
                iter_elem_to_erase = elem_iter;
                found_erase_elem   = true;
            }
        }

        if (found_erase_elem)
            cache_.erase (iter_elem_to_erase);

        return found_erase_elem;
    }
};
} // namespace caches

#endif
