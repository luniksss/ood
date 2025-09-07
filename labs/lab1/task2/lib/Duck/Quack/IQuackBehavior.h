#ifndef IQUACKBEHAVIOR_H
#define IQUACKBEHAVIOR_H

struct IQuackBehavior
{
    virtual ~IQuackBehavior(){};
    virtual void Quack() = 0;
};

#endif