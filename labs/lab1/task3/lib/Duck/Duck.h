#ifndef DUCK_H
#define DUCK_H

#include "Fly/FlyBehavior.h"
#include "Quack//QuackBehavior.h"
#include "Dance/DanceBehavior.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <functional>

using FlyBehavior = std::function<void()>;
using QuackBehavior = std::function<void()>;
using DanceBehavior = std::function<void()>;

class Duck
{
public:
  Duck(const FlyBehavior& flyBehavior,
       const QuackBehavior& quackBehavior,
       const DanceBehavior& danceBehavior)
  {
    SetFlyBehavior(flyBehavior);
    SetQuackBehavior(quackBehavior);
    SetDanceBehavior(danceBehavior);
  }

  void Quack() const
  {
    m_quackBehavior();
  }

  void Swim()
  {
    std::cout << "I'm swimming" << std::endl;
  }

  void Fly()
  {
    m_flyBehavior();
  }

  void Dance()
  {
    m_danceBehavior();
  }

  void SetFlyBehavior(FlyBehavior const& flyBehavior)
  {
    assert(flyBehavior);
    m_flyBehavior = flyBehavior;
  }

  void SetQuackBehavior(QuackBehavior const& quackBehavior)
  {
    assert(quackBehavior);
    m_quackBehavior = quackBehavior;
  }

  void SetDanceBehavior(DanceBehavior const& danceBehavior)
  {
    assert(danceBehavior);
    m_danceBehavior = danceBehavior;
  }

  virtual void Display() const = 0;
  virtual ~Duck() = default;

private:
  FlyBehavior m_flyBehavior;
  QuackBehavior m_quackBehavior;
  DanceBehavior m_danceBehavior;
};

#endif