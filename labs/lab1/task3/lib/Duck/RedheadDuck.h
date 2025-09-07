#ifndef REDHEADDUCK_H
#define REDHEADDUCK_H

#include "Duck.h"

class RedheadDuck : public Duck
{
public:
    RedheadDuck()
        : Duck(FlyWithWings(), QuackByQuack, DanceMinuet)
    {
    }

    void Display() const override
    {
        std::cout << "I'm redhead duck" << std::endl;
    }
};

#endif