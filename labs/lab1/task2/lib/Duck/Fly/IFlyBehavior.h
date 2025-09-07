#ifndef LAB01_SIMUDUCK_IFLYBEHAVIOR_H
#define LAB01_SIMUDUCK_IFLYBEHAVIOR_H

struct IFlyBehavior
{
    virtual ~IFlyBehavior(){};
    virtual void Fly() = 0;
    virtual bool IsFlying() const = 0;
    virtual int GetFlightCounter() const = 0;
};

#endif