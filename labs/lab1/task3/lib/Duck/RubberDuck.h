#ifndef RUBBERDUCK_H
#define RUBBERDUCK_H

#include "Duck.h"

class RubberDuck : public Duck
{
public:
    RubberDuck()
        : Duck(FlyNoWay, QuackBySqueak, DanceNoWay)
    {
    }

    void Display() const override
    {
        std::cout << "I'm rubber duck" << std::endl;
    }
};

#endif