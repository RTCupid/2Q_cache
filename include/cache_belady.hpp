#ifndef PERFECT_CACHE_H
#define PERFECT_CACHE_H

#include <iostream>
#include <vector>
#include <functional>

template <typename KeyT, typename ElemT>
class belady_cache
{
    using KeyVectorT  = std::vector<KeyT>;
    using FuncToGetElem = std::function<ElemT(const KeyT &)>;

    size_t size_;

    KeyVectorT  input_elements_;
    size_t      ind_elems_;
    FuncToGetElem slow_get_elem_;
    std::unordered_map<KeyT, ElemT> cache_;

public:
    belady_cache (size_t size, FuncToGetElem slow_get_elem, KeyVectorT &input_elements) :
        size_ (size),
        slow_get_elem_ (slow_get_elem),
        input_elements_ (input_elements),
        ind_elems_ (0) {}

    bool lookup_update (const KeyT &key)
    {
        ++ind_elems_;

        if (cache_.contains (key))
            return true;

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
        size_t distance_elem_to_erase = 0;
        KeyT   key_elem_to_erase = 0;

        size_t distance_input_elem = 0;

        bool not_using_elem = false;

        if (ind_elems_ == input_elements_.size ())
            return false;

        for (auto index = ind_elems_; index < input_elements_.size (); index++)
        {
            assert (index < input_elements_.size ());

            if (input_elements_[index] == key_input_elem)
            {
                distance_input_elem = index - ind_elems_;
                break;
            }

            if (index == input_elements_.size () - 1)
                return false;
        }

        for (auto &elem : cache_)
        {
            for (auto index = ind_elems_; index < input_elements_.size (); index++)
            {
                assert (index < input_elements_.size ());

                if (input_elements_[index] == elem.first)
                {
                    if (index - ind_elems_ > distance_elem_to_erase)
                    {
                        key_elem_to_erase      = elem.first;
                        distance_elem_to_erase = index - ind_elems_;
                    }

                    break;
                }

                if (index == input_elements_.size () - 1)
                {
                    key_elem_to_erase      = elem.first;
                    distance_elem_to_erase = index - ind_elems_;
                    not_using_elem         = true;
                    break;
                }
            }

            if (not_using_elem)
                break;
        }

        if (distance_elem_to_erase > distance_input_elem)
        {
            cache_.erase (key_elem_to_erase);
            return true;
        }

        return false;
    }
};

#endif
