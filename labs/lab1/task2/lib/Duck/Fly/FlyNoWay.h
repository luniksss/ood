#ifndef FLYNOWAY_H
#define FLYNOWAY_H

#include "IFlyBehavior.h"

class FlyNoWay : public IFlyBehavior
{
public:
    void Fly() override {}

    bool IsFlying() const override
    {
      return false;
    }

    int GetFlightCounter() const override
    {
      return 0;
    }
};

#endif