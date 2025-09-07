#ifndef DANCEBEHAVIOR_H
#define DANCEBEHAVIOR_H

#include <functional>
#include <iostream>

using DanceBehavior = std::function<void()>;

inline void DanceNoWay() {}

inline void DanceWaltz()
{
  std::cout << "I'm dancing waltz!!" << std::endl;
}

inline void DanceMinuet()
{
  std::cout << "I'm dancing minuet!!" << std::endl;
}

#endif //DANCEBEHAVIOR_H
