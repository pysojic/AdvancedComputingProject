#include "include/Matrix.hpp"
#include "include/Vector.hpp"
#include "include/StopWatch.hpp"

#include <random>

int main()
{
    ScopedTimer st("Program duration");
    Matrix<int, 100, 100> m(3);
    Matrix<int, 100, 100> m2(4);
    auto m3 = m - m2;
    m.print();
    m2.print();
    m3.print();
}