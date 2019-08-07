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


template <typename T, typename S, typename = void>
struct common_base_helper
{
    using type = void;
};

template <typename T>
struct common_base_helper<T, T>
{
    using type = T;
};

template <typename T, typename S>
struct common_base_helper<T, S, std::enable_if_t<!std::is_same_v<T, S> && std::is_base_of_v<T, S>>>
{
    using type = T;
};

template <typename T, typename S>
struct common_base_helper<T, S, std::enable_if_t<!std::is_same_v<T, S> && std::is_base_of_v<S, T>>>
{
    using type = S;
};

template <typename T, typename ...REST>
struct common_base
{
    using type = typename common_base_helper<T, typename common_base<REST...>::type>::type;
};

template <typename T>
struct common_base<T>
{
    using type = T;
};

template <typename ...TYPES>
using common_base_t = typename common_base<TYPES...>::type;

}

template <typename ...ITERATORS>
class Iterator
{
public:
    using difference_type   = std::ptrdiff_t; // TODO: determine common difference type
    using iterator_category = type_traits::common_base_t<typename std::iterator_traits<ITERATORS>::iterator_category...>;
    using value_type        = std::tuple<typename std::iterator_traits<ITERATORS>::value_type...>;
    using reference         = std::tuple<typename std::iterator_traits<ITERATORS>::reference...>;
    using pointer           = std::tuple<typename std::iterator_traits<ITERATORS>::pointer...>;

public:
    Iterator() = default;

    explicit Iterator(ITERATORS&& ... iterators)
            : _iterators(std::forward<ITERATORS>(iterators)...)
    {
        static_assert(!std::is_void_v<iterator_category>);
    }

    reference operator*() const
    {
        return std::apply([](auto&& ...item) { return reference{*item...}; }, _iterators);
    }

    Iterator& operator++()
    {
        std::apply([](auto&& ...item) { std::make_tuple(++item...); }, _iterators);
        return *this;
    }

    const Iterator operator++(int)
    {
        return std::apply([](auto&& ...item) { return Iterator{item++...}; }, _iterators);
    }

    Iterator& operator+=(Iterator::difference_type difference)
    {
        std::apply([&](auto&& ...item) { std::make_tuple(item += difference...); }, _iterators);
        return *this;
    }

    Iterator operator+(Iterator::difference_type difference) const
    {
        return std::apply([&](auto&& ...item) { return Iterator{item + difference...}; }, _iterators);
    }

    friend Iterator operator+(Iterator::difference_type difference, const Iterator& iterator)
    {
        return std::apply([&](auto&& ...item) { return Iterator{difference + item...}; }, iterator._iterators);
    }

    Iterator& operator--()
    {
        std::apply([](auto&& ...item) { std::make_tuple(--item...); }, _iterators);
        return *this;
    }

    const Iterator operator--(int)
    {
        return std::apply([](auto&& ...item) { return Iterator{item--...}; }, _iterators);
    }

    Iterator& operator-=(Iterator::difference_type difference)
    {
        return *this += (-difference);
    }

    Iterator operator-(Iterator::difference_type difference) const
    {
        return std::apply([&](auto&& ...item) { return Iterator{item - difference...}; }, _iterators);
    }

    difference_type operator-(Iterator other) const
    {
        return diff(other, std::make_index_sequence<sizeof...(ITERATORS)>());
    }

    reference operator[](Iterator::difference_type index)
    {
        return std::apply([&](auto&& ...item) { return reference{item[index]...}; }, _iterators);
    }

    bool operator==(const Iterator& other) const
    {
        return eq(other, std::make_index_sequence<sizeof...(ITERATORS)>());
    }

    bool operator!=(const Iterator& other) const
    {
        return !(*this == other);
    }

    bool operator<(const Iterator& other) const
    {
        return (other - *this) > 0;
    }

    bool operator>(const Iterator& other) const
    {
        return other < *this;
    }

    bool operator>=(const Iterator& other) const
    {
        return !(*this < other);
    }

    bool operator<=(const Iterator& other) const
    {
        return !(*this > other);
    }

private:
    template <size_t ...INDICES>
    bool eq(const Iterator& other, std::index_sequence<INDICES...>) const
    {
        return ((std::get<INDICES>(_iterators) == std::get<INDICES>(other._iterators)) && ...);
    }

    template <size_t ...INDICES>
    difference_type diff(const Iterator& other, std::index_sequence<INDICES...>) const
    {
        const auto differences = std::tuple{(std::get<INDICES>(_iterators) - std::get<INDICES>(other._iterators))...};
        const auto& difference = std::get<0>(differences);
        if (((std::get<INDICES>(differences) != difference) || ...))
        {
            throw std::logic_error("Inconsistent distance");
        }
        return difference;
    }

private:
    std::tuple<ITERATORS...> _iterators;
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
        return std::apply([](auto&& ...item) { return Iterator{std::begin(item)...}; }, _iterables);
    }

    auto end()
    {
        return std::apply([](auto&& ...item) { return Iterator{std::end(item)...}; }, _iterables);
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
