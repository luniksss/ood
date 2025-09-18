#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../shapes/Shape.h"
#include "../picture/Picture.h"
#include "../shapes/strategy/IShapeStrategy.h"

using namespace shapes;
using namespace gfx;
using ::testing::_;
using ::testing::Return;
using ::testing::Invoke;

class MockShapeStrategy : public IShapeStrategy
{
public:
    MOCK_METHOD(void, Move, (double dx, double dy), (override));
    MOCK_METHOD(std::string, GetShapeName, (), (const, override));
    MOCK_METHOD(std::string, ShowInfo, (), (const, override));
    MOCK_METHOD(void, Draw, (gfx::ICanvas& canvas, Color color), (const, override));
};

class DummyCanvas : public ICanvas
{
public:
    void SetColor(Color color) override {}
    void MoveTo(double x, double y) override {}
    void LineTo(double x, double y) override {}
    void DrawEllipse(double cx, double cy, double rx, double ry) override {}
    void DrawText(double left, double top, double fontSize, const std::string &text) override {}
    void DrawRectangle(Point topLeft, double width, double height) override {}
    void DrawTriangle(Point vertex1, Point vertex2, Point vertex3) override {}
    void Display() override {}
};

TEST(ShapeTest, ConstructorAndGetters)
{
    auto mockStrategy = std::make_unique<MockShapeStrategy>();
    EXPECT_CALL(*mockStrategy, GetShapeName()).WillOnce(Return("MockShape"));
    EXPECT_CALL(*mockStrategy, ShowInfo()).WillOnce(Return("info"));

    const Shape shape("id123", 42, std::move(mockStrategy));

    EXPECT_EQ(shape.GetId(), "id123");
    EXPECT_EQ(shape.GetName(), "MockShape");
    EXPECT_EQ(shape.GetColor(), 42);
    EXPECT_EQ(shape.GetShapeInfo(), "info");
}

TEST(ShapeTest, MoveDelegatesToStrategy)
{
    auto mockStrategy = std::make_unique<MockShapeStrategy>();
    MockShapeStrategy* strategyPtr = mockStrategy.get();
    Shape shape("id", 0, std::move(mockStrategy));

    EXPECT_CALL(*strategyPtr, Move(5.0, 10.0)).Times(1);
    shape.Move(5.0, 10.0);
}

TEST(ShapeTest, DrawDelegatesToStrategy)
{
    auto mockStrategy = std::make_unique<MockShapeStrategy>();
    MockShapeStrategy* strategyPtr = mockStrategy.get();
    Shape shape("id", 12345, std::move(mockStrategy));
    DummyCanvas canvas;

    EXPECT_CALL(*strategyPtr, Draw(::testing::Ref(canvas), 12345)).Times(1);
    shape.Draw(canvas);
}

TEST(ShapeTest, SetColorAndSetShapeStrategy)
{
    auto mockStrategy1 = std::make_unique<MockShapeStrategy>();
    Shape shape("id", 10, std::move(mockStrategy1));

    shape.SetColor(20);
    EXPECT_EQ(shape.GetColor(), 20);

    auto mockStrategy2 = std::make_unique<MockShapeStrategy>();
    shape.SetShapeStrategy(std::move(mockStrategy2));
}

TEST(ShapeTest, MoveShapeAndDeleteShape)
{
    Picture picture;
    auto mockStrategy = std::make_unique<MockShapeStrategy>();
    MockShapeStrategy* strategyPtr = mockStrategy.get();
    auto shape = std::make_unique<Shape>("id", 1, std::move(mockStrategy));
    picture.AddShape("id", std::move(shape));

    EXPECT_CALL(*strategyPtr, Move(3.0, 4.0)).Times(1);
    picture.MoveShape("id", 3, 4);

    picture.DeleteShape("id");
    EXPECT_THROW(picture.MoveShape("id", 1, 1), std::invalid_argument);
}

TEST(ShapeTest, AddShapeDuplicateThrows)
{
    Picture picture;

    auto s1 = std::make_unique<Shape>("id1", 1, std::make_unique<MockShapeStrategy>());
    picture.AddShape("id1", std::move(s1));

    auto s2 = std::make_unique<Shape>("id1", 2, std::make_unique<MockShapeStrategy>());

    EXPECT_THROW(picture.AddShape("id1", std::move(s2)), std::invalid_argument);
}

TEST(ShapeTest, MoveShapeAndDeleteShapeWithMockShape)
{
    Picture picture;
    auto mockStrategy = std::make_unique<MockShapeStrategy>();
    MockShapeStrategy* strategyPtr = mockStrategy.get();
    auto shape = std::make_unique<Shape>("id", 1, std::move(mockStrategy));
    picture.AddShape("id", std::move(shape));

    EXPECT_CALL(*strategyPtr, Move(3.0, 4.0)).Times(1);
    picture.MoveShape("id", 3, 4);

    picture.DeleteShape("id");
    EXPECT_THROW(picture.MoveShape("id", 1, 1), std::invalid_argument);
}

TEST(ShapeTest, DrawShapeAndDrawPicture)
{
    Picture picture;
    DummyCanvas canvas;

    auto mockStrategy1 = std::make_unique<MockShapeStrategy>();
    MockShapeStrategy* s1ptr = mockStrategy1.get();
    auto shape1 = std::make_unique<Shape>("id1", 1, std::move(mockStrategy1));
    picture.AddShape("id1", std::move(shape1));

    EXPECT_CALL(*s1ptr, Draw(::testing::Ref(canvas), 1));
    picture.DrawShape("id1", canvas);

    // Adding second shape
    auto mockStrategy2 = std::make_unique<MockShapeStrategy>();
    MockShapeStrategy* s2ptr = mockStrategy2.get();
    auto shape2 = std::make_unique<Shape>("id2", 2, std::move(mockStrategy2));
    picture.AddShape("id2", std::move(shape2));

    EXPECT_CALL(*s1ptr, Draw(::testing::Ref(canvas), 1));
    EXPECT_CALL(*s2ptr, Draw(::testing::Ref(canvas), 2));

    picture.DrawPicture(canvas);
}

TEST(ShapeTest, ChangeColorAndChangeShape)
{
    Picture picture;
    auto mockStrategy = std::make_unique<MockShapeStrategy>();
    auto shape = std::make_unique<Shape>("id", 1, std::move(mockStrategy));
    picture.AddShape("id", std::move(shape));

    picture.ChangeColor("id", 100);
    EXPECT_EQ(picture.GetShapes().at("id")->GetColor(), 100);

    auto newMockStrategy = std::make_unique<MockShapeStrategy>();

    picture.ChangeShape("id", std::move(newMockStrategy));
}

TEST(ShapeTest, MovePicture)
{
    Picture picture;

    auto mockStrategy1 = std::make_unique<MockShapeStrategy>();
    MockShapeStrategy* s1ptr = mockStrategy1.get();
    auto shape1 = std::make_unique<Shape>("id1", 1, std::move(mockStrategy1));
    picture.AddShape("id1", std::move(shape1));

    auto mockStrategy2 = std::make_unique<MockShapeStrategy>();
    MockShapeStrategy* s2ptr = mockStrategy2.get();
    auto shape2 = std::make_unique<Shape>("id2", 2, std::move(mockStrategy2));
    picture.AddShape("id2", std::move(shape2));

    EXPECT_CALL(*s1ptr, Move(7, 8));
    EXPECT_CALL(*s2ptr, Move(7, 8));

    picture.MovePicture(7, 8);
}