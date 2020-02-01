#include "3d_triangle.hpp"
std::ostream& operator<<(std::ostream& os, const triangle& dt)
{
    for (int i = 0; i<2; i++){
        os << dt.p[i] <<" ";
    }
    return os;
}