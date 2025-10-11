#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../shapes/Point.h"
#include "../shapes/Shape.h"
#include "../shapes/ShapeFactory.h"
#include "../designer/Designer.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Invoke;

class MockDesigner : public IDesigner
{
public:
    MOCK_METHOD(PictureDraft, CreateDraft, (std::istream& input), (override));
};

class MockPainter : public IPainter
{
public:
    MOCK_METHOD(void, DrawPicture, (const PictureDraft& draft, ICanvas* canvas), (override));
};

class MockCanvas : public ICanvas
{
public:
    MOCK_METHOD(void, SetColor, (uint32_t color), (override));
    MOCK_METHOD(void, DrawLine, (const shapes::Point& from, const shapes::Point& to), (override));
    MOCK_METHOD(void, DrawEllipse, (const shapes::Point& center, double rx, double ry), (override));
};

class MockShapeFactory : public IShapeFactory
{
public:
    MOCK_METHOD(std::unique_ptr<shapes::Shape>, CreateShape, (std::string descr), (override));
};

class MockShape : public shapes::Shape
{
public:
    MockShape(uint32_t color = 0) : Shape(color) {}

    MOCK_METHOD(void, Draw, (ICanvas* canvas), (override));
    MOCK_METHOD(uint8_t, GetColor, (), (const));
};

class ClientTest : public testing::Test
{
protected:
    void SetUp() override
    {
        mockDesigner = std::make_unique<MockDesigner>();
        mockPainter = std::make_unique<MockPainter>();
        mockCanvas = std::make_unique<MockCanvas>();
        client = std::make_unique<Client>(mockDesigner.get(), mockPainter.get());
    }

    std::unique_ptr<MockDesigner> mockDesigner;
    std::unique_ptr<MockPainter> mockPainter;
    std::unique_ptr<MockCanvas> mockCanvas;
    std::unique_ptr<Client> client;
};

class DesignerTest : public testing::Test
{
protected:
    void SetUp() override
    {
        mockFactory = std::make_unique<MockShapeFactory>();
        designer = std::make_unique<Designer>(*mockFactory.get());
    }

    std::unique_ptr<MockShapeFactory> mockFactory;
    std::unique_ptr<Designer> designer;
};

class PainterTest : public testing::Test
{
protected:
    Painter painter;
    PictureDraft draft;
    MockCanvas mockCanvas;
};

class ShapeFactoryTest : public testing::Test
{
protected:
    ShapeFactory factory;
};

class PictureDraftTest : public testing::Test
{
protected:
    PictureDraft draft;
};

TEST_F(ClientTest, ValidInputCreatesDesignerAndPainter)
{
    std::stringstream input("rectangle 0x00FF00 10 10 50 50");
    PictureDraft draft;
    draft.AddShape(std::make_unique<MockShape>(0x00FF00));

    EXPECT_CALL(*mockDesigner, CreateDraft(_)).WillOnce(Return(draft));
    EXPECT_CALL(*mockPainter, DrawPicture(draft, mockCanvas.get()));

    client->ProcessInput(input, mockCanvas.get());
}

TEST_F(ClientTest, ClientHandleDesignerError)
{
    std::stringstream input("invalid input");

    EXPECT_CALL(*mockDesigner, CreateDraft(_)).WillOnce(testing::Throw(std::invalid_argument("Invalid shape description")));
    EXPECT_CALL(*mockPainter, DrawPicture(_, _)).Times(0);

    EXPECT_NO_THROW({
        client->ProcessInput(input, mockCanvas.get());
    });
}

TEST_F(ClientTest, ClientHandlePainterError)
{
    std::stringstream input("rectangle 0x00FF00 10 10 50 50");
    PictureDraft draft;
    draft.AddShape(std::make_unique<MockShape>(0x00FF00));

    EXPECT_CALL(*mockDesigner, CreateDraft(_)).WillOnce(Return(draft));
    EXPECT_CALL(*mockPainter, DrawPicture(_, _)).WillOnce(testing::Throw(std::runtime_error("Canvas error")));

    EXPECT_NO_THROW({
        client->ProcessInput(input, mockCanvas.get());
    });
}

TEST_F(ClientTest, EmptyInputDoesNotThrowError)
{
    std::stringstream input("");

    EXPECT_CALL(*mockDesigner, CreateDraft(_)).WillOnce(Return(PictureDraft()));
    EXPECT_CALL(*mockPainter, DrawPicture(_, _)).Times(1);

    testing::internal::CaptureStdout();
    EXPECT_NO_THROW({
        client->ProcessInput(input, mockCanvas.get());
    });
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

TEST_F(DesignerTest, CreateDraftWithSingleShape)
{
    std::stringstream input("rectangle 0x00FF00 10 10 50 50");
    auto mockShape = std::make_unique<MockShape>(0x00FF00);

    EXPECT_CALL(*mockFactory, CreateShape("rectangle 0x00FF00 10 10 50 50")).WillOnce(Return(testing::ByMove(std::move(mockShape))));

    auto draft = designer->CreateDraft(input);
    EXPECT_EQ(draft.GetShapeCount(), 1);
}

TEST_F(DesignerTest, CreateDraftWithMultipleShapes)
{
    std::stringstream input("rectangle 0x00FF00 10 10 50 50\ntriangle 0xFF0000 0 0 100 100 50 150");
    auto mockRect = std::make_unique<MockShape>(0x00FF00);
    auto mockTriangle = std::make_unique<MockShape>(0xFF0000);

    EXPECT_CALL(*mockFactory, CreateShape("rectangle 0x00FF00 10 10 50 50")).WillOnce(Return(testing::ByMove(std::move(mockRect))));
    EXPECT_CALL(*mockFactory, CreateShape("triangle 0xFF0000 0 0 100 100 50 150")).WillOnce(Return(testing::ByMove(std::move(mockTriangle))));

    auto draft = designer->CreateDraft(input);
    EXPECT_EQ(draft.GetShapeCount(), 2);
}

TEST_F(DesignerTest, CreateDraftWithEmptyInput)
{
    std::stringstream input("");
    auto draft = designer->CreateDraft(input);

    EXPECT_EQ(draft.GetShapeCount(), 0);
}

TEST_F(DesignerTest, CreateDraftWithInvalidShapeThrowsException)
{
    std::stringstream input("invalid_shape 0x0000FF 10 20");

    EXPECT_CALL(*mockFactory, CreateShape("invalid_shape 0x0000FF 10 20")).WillOnce(testing::Throw(std::invalid_argument("Unknown shape type")));

    EXPECT_THROW({
        designer->CreateDraft(input);
    }, std::invalid_argument);
}

TEST_F(DesignerTest, HandleFactoryThrowsException)
{
    std::stringstream input("rectangle 0x00FF00 10 10");

    EXPECT_CALL(*mockFactory, CreateShape("rectangle 0x00FF00 10 10")).WillOnce(testing::Throw(std::invalid_argument("Invalid parameters")));

    EXPECT_THROW({
        designer->CreateDraft(input);
    }, std::invalid_argument);
}

TEST_F(PainterTest, DrawPictureWithSingleShape)
{
    auto mockShape = std::make_unique<MockShape>(0x00FF00);
    EXPECT_CALL(*mockShape, Draw(&mockCanvas)).Times(1);
    EXPECT_CALL(*mockShape, GetColor()).WillOnce(Return(0x00FF00));

    draft.AddShape(std::move(mockShape));

    painter.DrawPicture(draft, &mockCanvas);
}

TEST_F(PainterTest, DrawPictureWithMultipleShapes)
{
    auto mockShape1 = std::make_unique<MockShape>(0xFF0000);
    auto mockShape2 = std::make_unique<MockShape>(0x0000FF);

    EXPECT_CALL(*mockShape1, Draw(&mockCanvas)).Times(1);
    EXPECT_CALL(*mockShape1, GetColor()).WillOnce(Return(0xFF0000));

    EXPECT_CALL(*mockShape2, Draw(&mockCanvas)).Times(1);
    EXPECT_CALL(*mockShape2, GetColor()).WillOnce(Return(0x0000FF));

    draft.AddShape(std::move(mockShape1));
    draft.AddShape(std::move(mockShape2));

    painter.DrawPicture(draft, &mockCanvas);
}

TEST_F(PainterTest, DrawPictureWithEmptyDraft)
{
    EXPECT_CALL(mockCanvas, SetColor(_)).Times(0);
    EXPECT_CALL(mockCanvas, DrawLine(_, _)).Times(0);
    EXPECT_CALL(mockCanvas, DrawEllipse(_, _, _)).Times(0);

    painter.DrawPicture(draft, &mockCanvas);
}

TEST_F(PainterTest, HandleCanvasThrowsException)
{
    auto mockShape = std::make_unique<MockShape>(0x00FF00);
    EXPECT_CALL(*mockShape, Draw(&mockCanvas)).WillOnce(testing::Throw(std::runtime_error("Canvas error")));
    EXPECT_CALL(*mockShape, GetColor()).WillOnce(Return(0x00FF00));

    draft.AddShape(std::move(mockShape));

    EXPECT_THROW({
        painter.DrawPicture(draft, &mockCanvas);
    }, std::runtime_error);
}

TEST_F(ShapeFactoryTest, CreateRectangle)
{
    auto shape = factory.CreateShape("rectangle 0x00FF00 10 10 50 50");

    ASSERT_NE(shape, nullptr);
    EXPECT_NE(dynamic_cast<shapes::Rectangle*>(shape.get()), nullptr);
    EXPECT_EQ(shape->GetColor(), 0x00FF00);
}

TEST_F(ShapeFactoryTest, CreateTriangle)
{
    auto shape = factory.CreateShape("triangle 0xFF0000 0 0 100 0 50 100");

    ASSERT_NE(shape, nullptr);
    EXPECT_NE(dynamic_cast<shapes::Triangle*>(shape.get()), nullptr);
    EXPECT_EQ(shape->GetColor(), 0xFF0000);
}

TEST_F(ShapeFactoryTest, CreateEllipse)
{
    auto shape = factory.CreateShape("ellipse 0x0000FF 100 100 50 30");

    ASSERT_NE(shape, nullptr);
    EXPECT_NE(dynamic_cast<shapes::Ellipse*>(shape.get()), nullptr);
    EXPECT_EQ(shape->GetColor(), 0x0000FF);
}

TEST_F(ShapeFactoryTest, CreateLine)
{
    auto shape = factory.CreateShape("line 0xFFFF00 0 0 100 100");

    ASSERT_NE(shape, nullptr);

    EXPECT_NE(dynamic_cast<shapes::Line*>(shape.get()), nullptr);
    EXPECT_EQ(shape->GetColor(), 0xFFFF00);
}

TEST_F(ShapeFactoryTest, CreateUnknownShapeThrowsException)
{
    EXPECT_THROW({
        factory.CreateShape("unknown_shape pink 10 20 30");
    }, std::invalid_argument);
}

TEST_F(ShapeFactoryTest, CreateShapeWithInvalidParameterCountThrowsException)
{
    EXPECT_THROW({
        factory.CreateShape("rectangle 0x00FF00 10 10");
    }, std::invalid_argument);

    EXPECT_THROW({
        factory.CreateShape("triangle 0xFF0000 0 0 100 0 50");
    }, std::invalid_argument);
}

TEST_F(PictureDraftTest, GetShapeCountAfterAddingShapeReturnsCorrectCount)
{
    EXPECT_EQ(draft.GetShapeCount(), 0);

    draft.AddShape(std::make_unique<MockShape>());
    EXPECT_EQ(draft.GetShapeCount(), 1);

    draft.AddShape(std::make_unique<MockShape>());
    EXPECT_EQ(draft.GetShapeCount(), 2);
}

TEST_F(PictureDraftTest, GetShapeWithValidIndexReturnsCorrectShape)
{
    auto mockShape1 = std::make_unique<MockShape>();
    auto mockShape2 = std::make_unique<MockShape>();

    auto* shape1Ptr = mockShape1.get();
    auto* shape2Ptr = mockShape2.get();

    draft.AddShape(std::move(mockShape1));
    draft.AddShape(std::move(mockShape2));

    EXPECT_EQ(draft.GetShape(0), shape1Ptr);
    EXPECT_EQ(draft.GetShape(1), shape2Ptr);
}

TEST_F(PictureDraftTest, GetShapeWithInvalidIndexThrowsException)
{
    draft.AddShape(std::make_unique<MockShape>());

    EXPECT_THROW({
        draft.GetShape(-1);
    }, std::out_of_range);

    EXPECT_THROW({
        draft.GetShape(1);
    }, std::out_of_range);
}

TEST_F(PictureDraftTest, AddShapeIncreasesShapeCount)
{
    size_t initialCount = draft.GetShapeCount();

    draft.AddShape(std::make_unique<MockShape>());
    EXPECT_EQ(draft.GetShapeCount(), initialCount + 1);

    draft.AddShape(std::make_unique<MockShape>());
    EXPECT_EQ(draft.GetShapeCount(), initialCount + 2);
}