#include "include/Matrix.hpp"
#include "include/Vector.hpp"

#include <random>

int main()
{
    std::mt19937_64 mt;
    std::uniform_int_distribution unif(0,10);

    Matrix<int, 2, 2> m(unif, mt);
    Matrix<int, 2, 2> m2 = m * 4 ;
    m.print();
    m2.print();
}