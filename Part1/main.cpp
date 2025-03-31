#include "include/Matrix.hpp"
#include "include/Vector.hpp"

#include <random>

int main()
{
    std::mt19937_64 mt;
    std::uniform_int_distribution unif(0,10);

    Matrix<int, 2, 2> m(3);
    Matrix<int, 2, 2> m2(4);
    auto m3 = m - m2;
    m.print();
    m2.print();
    m3.print();
}