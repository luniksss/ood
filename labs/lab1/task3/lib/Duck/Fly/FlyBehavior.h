#ifndef FLYBEHAVIOR_H
#define FLYBEHAVIOR_H

#include <functional>
#include <iostream>

using FlyBehavior = std::function<void()>;

inline void FlyNoWay() {}

inline FlyBehavior FlyWithWings()
{
  int flyCounter = 0;
  return [flyCounter]() mutable->void
  {
    flyCounter++;
    std::cout << "I'm flying with wings!! Flight Counter: " << flyCounter << std::endl;
  };
}

#endif //FLYBEHAVIOR_H
