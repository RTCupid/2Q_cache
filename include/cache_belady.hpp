#ifndef PERFECT_CACHE_H
#define PERFECT_CACHE_H

#include <iostream>
#include <vector>
#include <functional>

template <typename KeyT, typename ElemT>
class belady_cache
{
    size_t size_;
    size_t number_input_elements;

    using KeyVectorT  = std::vector<KeyT>;
    using FuncToGetElem = std::function<ElemT(const KeyT &)>;

    KeyVectorT  input_elements_;
    size_t      ind_elems_;
    FuncToGetElem slow_get_elem_;
    std::unordered_map<KeyT, ElemT> cache_;

public:
    belady_cache (size_t size, FuncToGetElem slow_get_elem, KeyVectorT input_elements) :
        size_ (size),
        slow_get_elem_ (slow_get_elem),
        input_elements_ (input_elements)  {}

    bool lookup_update (const KeyT &key)
    {
        if (cache_.contains (key))
            return true;

        if (cache_.size () == size_)
        {
            erase_elem_from_belady_cache ();
        }

        cache_.try_emplace (key, slow_get_elem_ (key));

        return false;
    }

private:
    void erase_elem_from_belady_cache ()
    {
        struct
        {
            size_t distance;
            size_t key;
        } elem_to_erase;

        for (auto &elem : cache_)
        {
            auto key_elem = elem.first;
            for (auto index = ind_elems_; index < number_input_elements; index++)
            {
                if (input_elements_.at(index) == key_elem)
                {
                    if (index - ind_elems_ > elem_to_erase.distance)
                    {
                        elem_to_erase.key      = key_elem;
                        elem_to_erase.distance = index - ind_elems_;
                    }
                    break;
                }
            }
        }

        cache_.erase (elem_to_erase.key);
    }
};

#endif
