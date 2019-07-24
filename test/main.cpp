#include <zip/zip.h>
#include <array>
#include <vector>
#include <list>
#include <iostream>

int main()
{
    // Compilation test
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

    for (auto&&[ae, ve, le] : zip::zip(a, v, l))
    {
        ae = 35.4;
        ve = 12;
        le = 'D';
    }

    for (const auto&[ae, ve, le, cae, cve, cle] : zip::zip(a, v, l, ca, cv, cl))
    {
        std::cout << ae << " " << ve << " " << le << " " << cae << " " << cve << " " << cle << "\n";
    }
}
