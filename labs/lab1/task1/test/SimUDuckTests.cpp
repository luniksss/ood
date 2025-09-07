#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../lib/Duck/Duck.h"
#include "../lib/Duck/Fly/FlyNoWay.h"
#include "../lib/Duck/Quack/MuteQuackBehavior.h"

class MockDanceBehavior : public IDanceBehavior
{
public:
    MOCK_METHOD(void, Dance, (), (override));
};

class TestDuck final : public Duck
{
public:
    explicit TestDuck(std::unique_ptr<IDanceBehavior>&& danceBehavior)
        : Duck(
            std::make_unique<FlyNoWay>(),
            std::make_unique<MuteQuackBehavior>(),
            std::move(danceBehavior)
            )
    {
    }

    void Display() const override
    {
        std::cout << "I'm test duck" << std::endl;
    }
};

TEST(SimUDuckTests, DuckDance)
{
    auto mockDanceBehavior = std::make_unique<MockDanceBehavior>();

    EXPECT_CALL(*mockDanceBehavior, Dance()).Times(1);

    TestDuck duck(std::move(mockDanceBehavior));
    duck.Dance();
}

TEST(SimUDuckTests, DuckNotDance)
{
    auto mockDanceBehavior = std::make_unique<MockDanceBehavior>();

    EXPECT_CALL(*mockDanceBehavior, Dance()).Times(0);

    TestDuck duck(std::move(mockDanceBehavior));
    duck.Display();
}