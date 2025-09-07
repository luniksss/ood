#ifndef DECOYDUCK_H
#define DECOYDUCK_H

#include "Duck.h"

class DecoyDuck : public Duck
{
public:
    DecoyDuck()
        : Duck(FlyNoWay, QuackNoWay, DanceNoWay)
    {
    }

    void Display() const override
    {
        std::cout << "I'm decoy duck" << std::endl;
    }
};

#endif