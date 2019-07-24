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
    const std::array<double, 3> ca{65.3, 2.4, 3.5};
    const std::vector<int> cv{65, 2, 3};
    const std::list<char> cl{'A', 'B', 'C'};

    for (const auto&[ae, ve, le, cae, cve, cle] : zip::zip(a, v, l, ca, cv, cl))
    {
        std::cout << ae << " " << ve << " " << le << " " << cae << " " << cve << " " << cle << "\n";
    }
}
