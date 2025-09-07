#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../lib/Duck/Duck.h"
#include "./MockFlyBehavior.h"
#include "./MockQuackBehavior.h"
#include "../lib/Duck/Dance/DanceNoWay.h"

class TestDuck final : public Duck
{
public:
    explicit TestDuck(
        std::unique_ptr<IFlyBehavior>&& flyBehavior,
        std::unique_ptr<IQuackBehavior>&& quackBehavior)
        : Duck(
            std::move(flyBehavior),
            std::move(quackBehavior),
            std::make_unique<DanceNoWay>()
            )
    {
    }

    void Display() const override
    {
        std::cout << "I'm test duck" << std::endl;
    }
};


void expectedFlyCalls(const std::unique_ptr<MockFlyBehavior> &mockFlyBehavior, int flightCounter, int numFlights, bool isFlying)
{
    EXPECT_CALL(*mockFlyBehavior, GetFlightCounter())
        .WillRepeatedly(::testing::Return(flightCounter));
    EXPECT_CALL(*mockFlyBehavior, IsFlying())
        .WillRepeatedly(testing::Return(isFlying));
    EXPECT_CALL(*mockFlyBehavior, Fly()).Times(numFlights);
}

void expectedQuackCalls(const std::unique_ptr<MockQuackBehavior> &mockQuackBehavior, int quackCounter)
{
    EXPECT_CALL(*mockQuackBehavior, Quack()).Times(quackCounter);
}

TEST(SimUDuckTest, FlyingDuckQuacksOnEvenFlights)
{
    auto mockFlyBehavior = std::make_unique<MockFlyBehavior>();
    auto mockQuackBehavior = std::make_unique<MockQuackBehavior>();

    int startFlightCounter = 1;
    int startQuackCounter = 0;
    expectedFlyCalls(mockFlyBehavior, startFlightCounter, 1, true);
    expectedQuackCalls(mockQuackBehavior, startQuackCounter);

    TestDuck duck(std::move(mockFlyBehavior), std::move(mockQuackBehavior));
    duck.Fly();

    startFlightCounter++;
    startQuackCounter++;
    auto newMockFlyBehavior = std::make_unique<MockFlyBehavior>();
    auto newMockQuackBehavior = std::make_unique<MockQuackBehavior>();

    expectedFlyCalls(newMockFlyBehavior, startFlightCounter, 1, true);
    expectedQuackCalls(newMockQuackBehavior, startQuackCounter);

    TestDuck quackingDuck(std::move(newMockFlyBehavior), std::move(newMockQuackBehavior));
    quackingDuck.Fly();
}

TEST(SimUDuckTest, NotFlyingDuckDoesNotQuackOnEvenFlights)
{
    auto mockFlyBehavior = std::make_unique<MockFlyBehavior>();
    auto mockQuackBehavior = std::make_unique<MockQuackBehavior>();

    expectedFlyCalls(mockFlyBehavior, 0, 2, false);
    expectedQuackCalls(mockQuackBehavior, 0);

    TestDuck duck(std::move(mockFlyBehavior), std::move(mockQuackBehavior));
    duck.Fly();
    duck.Fly();
}