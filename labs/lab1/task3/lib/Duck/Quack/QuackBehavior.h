#ifndef QUACKBEHAVIOR_H
#define QUACKBEHAVIOR_H

#include <functional>
#include <iostream>

using QuackBehavior = std::function<void()>;

inline void QuackNoWay() {}

inline void QuackByQuack()
{
    std::cout << "Quack Quack!!!" << std::endl;
}

inline void QuackBySqueak()
{
    std::cout << "Squeak!!!" << std::endl;
}

#endif //QUACKBEHAVIOR_H
