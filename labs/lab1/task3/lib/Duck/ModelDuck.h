#ifndef MODELDUCK_H
#define MODELDUCK_H

#include "Duck.h"

class ModelDuck : public Duck
{
public:
    ModelDuck()
        : Duck(FlyNoWay, QuackByQuack, DanceNoWay)
    {
    }

    void Display() const override
    {
        std::cout << "I'm model duck" << std::endl;
    }
};

#endif