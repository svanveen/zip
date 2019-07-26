#include <zip/zip.h>
#include <array>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

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
}
