#ifndef DUCK_H
#define DUCK_H

#include "Fly/IFlyBehavior.h"
#include "Quack/IQuackBehavior.h"
#include "Dance/IDanceBehavior.h"

#include <cassert>
#include <iostream>
#include <memory>

const int EVEN_FLIGHT_NUMBER = 2;
const int ZERO_DIVIDING_REMAINDER = 0;

class Duck
{
public:
  Duck(std::unique_ptr<IFlyBehavior>&& flyBehavior,
       std::unique_ptr<IQuackBehavior>&& quackBehavior,
       std::unique_ptr<IDanceBehavior>&& danceBehavior)
       : m_quackBehavior(std::move(quackBehavior))
  {
    assert(m_quackBehavior);
    SetFlyBehavior(std::move(flyBehavior));
    SetDanceBehavior(std::move(danceBehavior));
  }

  void Quack() const
  {
    m_quackBehavior->Quack();
  }

  void Swim()
  {
    std::cout << "I'm swimming" << std::endl;
  }

  void Fly()
  {
    const bool isFlying =  m_flyBehavior->IsFlying();
    m_flyBehavior->Fly();

    if (isFlying && isEvenFlightNumber())
    {
      m_quackBehavior->Quack();
    }
  }

  //не константный, потому что Dance может изменять состояние объекта стратегии(например, счётчики танцевальных движений и т.п.)
  void Dance()
  {
    m_danceBehavior->Dance();
  }

  void SetFlyBehavior(std::unique_ptr<IFlyBehavior>&& flyBehavior)
  {
    assert(flyBehavior);
    m_flyBehavior = std::move(flyBehavior);
  }

  void SetDanceBehavior(std::unique_ptr<IDanceBehavior>&& danceBehavior)
  {
    assert(danceBehavior);
    m_danceBehavior = std::move(danceBehavior);
  }

  virtual void Display() const = 0;
  virtual ~Duck() = default;

private:
  std::unique_ptr<IFlyBehavior> m_flyBehavior;
  std::unique_ptr<IQuackBehavior> m_quackBehavior;
  std::unique_ptr<IDanceBehavior> m_danceBehavior;

  bool isEvenFlightNumber()
  {
    return m_flyBehavior->GetFlightCounter() % EVEN_FLIGHT_NUMBER == ZERO_DIVIDING_REMAINDER;
  }
};

#endif