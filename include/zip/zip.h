#ifndef ZIP_ZIP_H
#define ZIP_ZIP_H

#include <tuple>

namespace zip {

namespace type_traits {

template <class T>
struct remove_rvalue_reference
{
    using type = T;
};

template <class T>
struct remove_rvalue_reference<T&>
{
    using type = T&;
};

template <class T>
struct remove_rvalue_reference<T&&>
{
    using type = T;
};

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <class T>
using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

}

template <typename ...ITERATORS>
class Iterator
{
    using IteratorTuple = std::tuple<ITERATORS...>;
    using value_type = std::tuple<typename std::iterator_traits<ITERATORS>::value_type...>;
    using reference  = std::tuple<typename std::iterator_traits<ITERATORS>::reference...>;
    using pointer    = std::tuple<typename std::iterator_traits<ITERATORS>::pointer...>;
public:
    explicit Iterator(IteratorTuple iterators)
            : _iterators(std::move(iterators))
    {
    }

    reference operator*()
    {
        return std::apply([](auto&& ...item) { return reference{*item...}; }, _iterators);
    }

    Iterator& operator++()
    {
        std::apply([](auto&& ...item) { std::make_tuple(++item...); }, _iterators);
        return *this;
    }

    bool operator!=(const Iterator& other) const
    {
        return neq(other, std::make_index_sequence<sizeof...(ITERATORS)>());
    }

private:
    template <size_t ...INDICES>
    bool neq(const Iterator& other, std::index_sequence<INDICES...>) const
    {
        return ((std::get<INDICES>(_iterators) != std::get<INDICES>(other._iterators)) || ...); // TODO: logical || or &&
    }

private:
    IteratorTuple _iterators;
};

template <typename ...ITERABLE>
class View
{
public:
    explicit View(ITERABLE&& ...iterable)
            : _iterables(std::forward<ITERABLE>(iterable)...)
    {
    }

    auto begin()
    {
        return Iterator{std::apply([](auto&& ...item) { return std::make_tuple(std::begin(item)...); }, _iterables)};
    }

    auto end()
    {
        return Iterator{std::apply([](auto&& ...item) { return std::make_tuple(std::end(item)...); }, _iterables)};
    }


private:
    std::tuple<type_traits::remove_rvalue_reference_t<ITERABLE&&> ...> _iterables;
};


template <typename ...ITERABLE>
View<ITERABLE...> zip(ITERABLE&& ...iterable)
{
    return View<ITERABLE...>{std::forward<ITERABLE>(iterable)...};
}

template <typename ITERABLE>
ITERABLE zip(ITERABLE&& iterable)
{
    return std::forward<ITERABLE>(iterable);
}


template <typename ...ITERABLE>
auto begin(ITERABLE&& ...iterable)
{
    static_assert((std::is_lvalue_reference_v<ITERABLE> && ...), "begin() must be called with an lvalue reference");
    return View<ITERABLE...>{std::forward<ITERABLE>(iterable)...}.begin();
}

template <typename ITERABLE>
auto begin(ITERABLE&& iterable)
{
    static_assert(std::is_lvalue_reference_v<ITERABLE>, "begin() must be called with an lvalue reference");
    return std::begin(iterable);
}


template <typename ...ITERABLE>
auto end(ITERABLE&& ...iterable)
{
    static_assert((std::is_lvalue_reference_v<ITERABLE> && ...), "begin() must be called with an lvalue reference");
    return View<ITERABLE...>{std::forward<ITERABLE>(iterable)...}.end();
}

template <typename ITERABLE>
auto end(ITERABLE&& iterable)
{
    static_assert(std::is_lvalue_reference_v<ITERABLE>, "begin() must be called with an lvalue reference");
    return std::end(iterable);
}

}

#endif //ZIP_ZIP_H
