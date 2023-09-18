#ifndef STATES_H
#define STATES_H

#include <iostream>

enum class States
{
    x, o, empty
};

std::ostream& operator<<(std::ostream& output, States state);

#endif