#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H

#include "IFlyBehavior.h"
#include <iostream>

class FlyWithWings : public IFlyBehavior
{
public:
    void Fly() override
    {
      m_flightCounter++;
      std::cout << "I'm flying with wings!! Flight Counter: " << m_flightCounter << std::endl;
    }

    bool IsFlying() const override
    {
      return true;
    }

    int GetFlightCounter() const override
    {
      return m_flightCounter;
    }

private:
    int m_flightCounter = 0;
};

#endif