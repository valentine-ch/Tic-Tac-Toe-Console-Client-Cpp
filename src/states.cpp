#include "states.h"

std::ostream& operator<<(std::ostream& output, States state)
{
    switch (state)
    {
        case States::empty: output << ' '; break;
        case States::x: output << 'X'; break;
        case States::o: output << 'O'; break;
        default: output << "E"; break;
    }
    return output;
}