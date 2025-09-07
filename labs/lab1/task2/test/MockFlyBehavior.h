#ifndef MOCKFLYBEHAVIOR_H
#define MOCKFLYBEHAVIOR_H

#include <gmock/gmock.h>
#include "../lib/Duck/Fly/IFlyBehavior.h"

class MockFlyBehavior final : public IFlyBehavior
{
public:
    MOCK_METHOD(void, Fly, (), (override));
    MOCK_METHOD(bool, IsFlying, (), (const, override));
    MOCK_METHOD(int, GetFlightCounter, (), (const, override));
};

#endif //MOCKFLYBEHAVIOR_H
