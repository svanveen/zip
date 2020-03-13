#include <zip/zip.h>
#include <array>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

struct NonCopyable
{
    NonCopyable() = default;

    NonCopyable(NonCopyable&&) = default;

    NonCopyable& operator=(NonCopyable&&) = default;

    NonCopyable(const NonCopyable&) = delete;

    NonCopyable& operator=(const NonCopyable&) = delete;

    NonCopyable* begin()
    {
        return this;
    }

    NonCopyable* end()
    {
        return this;
    }
};

int main()
{
    std::array<double, 3> a{65.3, 2.4, 3.5};
    std::vector<int> v{65, 2, 3};
    std::list<char> l{'A', 'B', 'C'};

    const std::array<double, 3>& ca = a;
    const std::vector<int>& cv = v;
    const std::list<char>& cl = l;

    for (const auto&[ae, ve, le, cae, cve, cle] : zip::zip(a, v, l, ca, cv, cl))
    {
        std::cout << ae << " " << ve << " " << le << " " << cae << " " << cve << " " << cle << "\n";
    }

    // auto&& is a universal reference
    for (auto&&[ae, ve, le] : zip::zip(a, v, l))
    {
        ae = 35.4;
        ve = 12;
        le = 'D';
    }

    for (auto&&[e1, e2] : zip::zip(std::vector<double>{2.2, 3.3}, std::vector<int>{4, 5}))
    {
        std::cout << e1 << " " << e2 << "\n";
    }

    std::for_each(zip::begin(a, v, l), zip::end(a, v, l), [](auto t) {
        const auto[ae, ve, le] = t;
        std::cout << ae << " " << ve << " " << le << "\n";
    });

    for (const auto& z : zip::zip(a))
    {
        std::cout << z << "\n";
    }

    std::for_each(zip::begin(a), zip::end(a), [](auto t) {
        std::cout << t << "\n";
    });

    for (auto&& e : NonCopyable{})
    {}

    static_assert(std::is_same_v<std::iterator_traits<decltype(zip::begin(a, v, l))>::iterator_category, std::bidirectional_iterator_tag>);

    zip::zip(a) == zip::zip(a);

    // won't compile
    // zip::Iterator(std::back_inserter(v)) == zip::Iterator(std::back_inserter(v));

    zip::begin(a, v)++;
    zip::begin(a, v)+=2;
    zip::begin(a, v)+2;
    2+zip::begin(a, v);
    zip::end(a, v)--;
    zip::end(a, v)-=2;
    zip::end(a, v)-2;

    zip::begin(a, v) < zip::begin(a, v);
    zip::begin(a, v) > zip::begin(a, v);
    zip::begin(a, v) <= zip::begin(a, v);
    zip::begin(a, v) >= zip::begin(a, v);
    zip::begin(a, v) == zip::begin(a, v);
    zip::begin(a, v) != zip::begin(a, v);

    std::tuple<double, int> vt{42.6, 36};

    const auto dist = zip::end(a, v) - zip::begin(a, v);
    std::cout << dist << "\n";

    auto rt = zip::begin(a, v)[1];
    rt = vt;
    const auto[e1, e2] = rt;
    std::cout << e1 << " " << e2 << "\n";
}
