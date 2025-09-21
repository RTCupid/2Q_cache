#ifndef PERFECT_CACHE_H
#define PERFECT_CACHE_H

#include <iostream>
#include <vector>
#include <functional>
#include <queue>

template <typename KeyT, typename ElemT>
class belady_cache
{
    using KeyVectorT  = std::vector<KeyT>;
    using FuncToGetElem = std::function<ElemT(const KeyT &)>;

    size_t size_;
    size_t      current_index_;
    FuncToGetElem slow_get_elem_;
    std::unordered_map<KeyT, ElemT> cache_;
    std::unordered_map<KeyT, std::queue<size_t>> key_positions_;

public:
    belady_cache (size_t size, FuncToGetElem slow_get_elem, KeyVectorT &input_elements) :
        size_ (size),
        slow_get_elem_ (slow_get_elem),
        current_index_ (0)
    {
        for (size_t i = 0; i < input_elements.size (); ++i)
            key_positions_[input_elements[i]].push (i);
    }

    bool lookup_update (const KeyT &key)
    {
        ++current_index_;
        key_positions_[key].pop ();

        if (cache_.contains (key))
            return true;

        if (key_positions_[key].empty ())
            return false;

        if (cache_.size () == size_)
        {
            if (erase_elem_from_belady_cache (key))
                cache_.try_emplace (key, slow_get_elem_ (key));

            return false;
        }

        cache_.try_emplace (key, slow_get_elem_ (key));

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
        KeyT key_to_erase;
        size_t max_distance = key_positions_[key_input_elem].front () - current_index_;
        bool found_erase_elem = false;

        for (const auto& elem : cache_)
        {
            KeyT key = elem.first;

            if (key_positions_[key].empty ())
            {
                key_to_erase     = key;
                found_erase_elem = true;
                break;
            }

            size_t distance = key_positions_[key].front () - current_index_;

            if (distance > max_distance)
            {
                max_distance     = distance;
                key_to_erase     = key;
                found_erase_elem = true;
            }
        }

        if (found_erase_elem)
        {
            cache_.erase (key_to_erase);

            return true;
        }

        return false;
    }
};

#endif
