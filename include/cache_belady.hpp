#ifndef PERFECT_CACHE_H
#define PERFECT_CACHE_H

#include <iostream>
#include <vector>
#include <functional>

template <typename KeyT, typename ElemT>
class belady_cache
{
    size_t size_;
    size_t number_input_elements_;

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
        input_elements_ (input_elements),
        number_input_elements_ (input_elements_.size ()),
        ind_elems_ (0) {}

    bool lookup_update (const KeyT &key)
    {
        ++ind_elems_;

        if (cache_.contains (key))
            return true;

        if (cache_.size () == size_)
        {
            erase_elem_from_belady_cache ();
        }

        cache_.try_emplace (key, slow_get_elem_ (key));

        return false;
    }

    void dump ()
    {
        std::cout << "Dump:\n";

        for (const auto &elem : cache_)
            std::cout << elem.second << ' ';

        std::cout << '\n';
    }

private:
    void erase_elem_from_belady_cache ()
    {
        struct
        {
            size_t distance = 0;
            KeyT   key = 0;
        } elem_to_erase;

        std::cout << "ind_elems: " << ind_elems_ << "\n";

        for (auto &elem : cache_)
        {
            auto key_elem = elem.first;

            for (auto index = ind_elems_; index < input_elements_.size (); index++)
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
