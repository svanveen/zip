#ifndef ZIP_ZIP_H
#define ZIP_ZIP_H

#include <tuple>

namespace zip {

template <typename ...ITERABLE>
class View
{
public:
    template <typename ...ITERATORS>
    class Iterator
    {
        using IteratorTuple = std::tuple<ITERATORS...>;
        using ValueTuple = std::tuple<decltype(*std::declval<ITERATORS>())...>;
    public:
        explicit Iterator(IteratorTuple iterators)
                : _iterators(std::move(iterators))
        {
        }

        ValueTuple operator*()
        {
            return get(std::make_index_sequence<sizeof...(ITERABLE)>());
        }

        Iterator& operator++()
        {
            inc(std::make_index_sequence<sizeof...(ITERABLE)>());
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return neq(other, std::make_index_sequence<sizeof...(ITERABLE)>());
        }

    private:
        template <size_t ...INDICES>
        ValueTuple get(std::index_sequence<INDICES...>)
        {
            return ValueTuple{*std::get<INDICES>(_iterators)...};
        }

        template <size_t ...INDICES>
        void inc(std::index_sequence<INDICES...>)
        {
            std::make_tuple(++std::get<INDICES>(_iterators)...); // TODO: find way what this won't be optimized
        }

        template <size_t ...INDICES>
        bool neq(const Iterator& other, std::index_sequence<INDICES...>) const
        {
            return ((std::get<INDICES>(_iterators) != std::get<INDICES>(other._iterators)) || ...); // TODO: logical || or &&
        }

    private:
        IteratorTuple _iterators;
    };

public:
    explicit View(ITERABLE& ...iterable)
            : _iterables(iterable...)
    {
    }

    auto begin()
    {
        return begin(std::make_index_sequence<sizeof...(ITERABLE)>());
    }

    auto end()
    {
        return end(std::make_index_sequence<sizeof...(ITERABLE)>());
    }


private:
    template <size_t ...INDICES>
    auto begin(std::index_sequence<INDICES...>)
    {
        return Iterator{std::make_tuple(std::begin(std::get<INDICES>(_iterables))...)};
    }

    template <size_t ...INDICES>
    auto end(std::index_sequence<INDICES...>)
    {
        return Iterator{std::make_tuple(std::end(std::get<INDICES>(_iterables))...)};
    }

private:
    std::tuple<ITERABLE& ...> _iterables;
};


template <typename ...ITERABLE>
View<ITERABLE...> zip(ITERABLE& ...iterable)
{
    return View<ITERABLE...>{iterable...};
}
}

#endif //ZIP_ZIP_H
