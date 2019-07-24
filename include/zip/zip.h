#ifndef ZIP_ZIP_H
#define ZIP_ZIP_H

#include <tuple>

namespace zip {

template <typename ...ITERABLE>
class View
{
public:
    class Iterator
    {
        using IteratorTuple = std::tuple<decltype(std::begin(std::declval<ITERABLE>()))...>;
    public:
        explicit Iterator(IteratorTuple iterators)
                : _iterators(std::move(iterators))
        {
        }

        decltype(auto) operator*()
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
        decltype(auto) get(std::index_sequence<INDICES...>)
        {
            return std::make_tuple(*std::get<INDICES>(_iterators)...);
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

    Iterator begin()
    {
        return begin(std::make_index_sequence<sizeof...(ITERABLE)>());
    }

    Iterator end()
    {
        return end(std::make_index_sequence<sizeof...(ITERABLE)>());
    }

private:
    template <size_t ...INDICES>
    Iterator begin(std::index_sequence<INDICES...>)
    {
        return Iterator{std::make_tuple(std::begin(std::get<INDICES>(_iterables))...)};
    }

    template <size_t ...INDICES>
    Iterator end(std::index_sequence<INDICES...>)
    {
        return Iterator{std::make_tuple(std::end(std::get<INDICES>(_iterables))...)};
    }

private:
    std::tuple<ITERABLE& ...> _iterables;
};


template <typename ...ITERABLE>
View<ITERABLE...> zip(ITERABLE& ...iterable)
{
    return View(iterable...);
}
}

#endif //ZIP_ZIP_H
