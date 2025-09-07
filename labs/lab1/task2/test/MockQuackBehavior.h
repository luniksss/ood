#ifndef MOCKQUACKBEHAVIOR_H
#define MOCKQUACKBEHAVIOR_H

#include <gmock/gmock.h>
#include "../lib/Duck/Quack/IQuackBehavior.h"

class MockQuackBehavior final : public IQuackBehavior
{
public:
  MOCK_METHOD(void, Quack, (), (override));
};

#endif //MOCKQUACKBEHAVIOR_H
