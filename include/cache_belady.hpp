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
            erase_elem_from_belady_cache ();

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
    void erase_elem_from_belady_cache ()
    {
        struct
        {
            size_t distance = 0;
            KeyT   key = 0;
        } elem_to_erase;

        bool not_using_elem = false;

        if (ind_elems_ == input_elements_.size ())
        {
            auto first_iterator = cache_.begin();
            cache_.erase(first_iterator);
            return;
        }

        for (auto &elem : cache_)
        {
            for (auto index = ind_elems_; index < input_elements_.size (); index++)
            {
                assert (index < input_elements_.size ());

                if (input_elements_[index] == elem.first)
                {
                    if (index - ind_elems_ > elem_to_erase.distance)
                    {
                        elem_to_erase.key      = elem.first;
                        elem_to_erase.distance = index - ind_elems_;
                    }

                    break;
                }

                if (index == input_elements_.size () - 1)
                {
                    elem_to_erase.key      = elem.first;
                    not_using_elem = true;
                }
            }

            if (not_using_elem)
                break;
        }

        cache_.erase (elem_to_erase.key);
    }
};

#endif
