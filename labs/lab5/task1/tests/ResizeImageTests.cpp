#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../document/command/ResizeImageCommand.h"

class MockImage : public IImage
{
public:
    MOCK_METHOD(std::string, GetPath, (), (const, override));
    MOCK_METHOD(unsigned, GetWidth, (), (const, override));
    MOCK_METHOD(unsigned, GetHeight, (), (const, override));
    MOCK_METHOD(void, Resize, (unsigned width, unsigned height), (override));
};

class ResizeImageCommandTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mockImage = std::make_shared<MockImage>();
        items.push_back(CDocumentItem(mockImage));
    }

    std::shared_ptr<MockImage> mockImage;
    std::vector<CDocumentItem> items;
    const size_t validPosition = 0;
    const unsigned newWidth = 200;
    const unsigned newHeight = 150;
};

TEST_F(ResizeImageCommandTest, Execute_Success)
{
    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, Resize(newWidth, newHeight)).Times(1);

    ResizeImageCommand command(validPosition, newWidth, newHeight, items);
    EXPECT_NO_THROW(command.DoExecute());
}

TEST_F(ResizeImageCommandTest, Unexecute_Success)
{
    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, Resize(newWidth, newHeight)).Times(1);
    EXPECT_CALL(*mockImage, Resize(100, 100)).Times(1);

    ResizeImageCommand command(validPosition, newWidth, newHeight, items);
    command.DoExecute();
    command.DoUnexecute();
}

TEST_F(ResizeImageCommandTest, Execute_OutOfRange)
{
    constexpr size_t invalidPosition = 1;
    ResizeImageCommand command(invalidPosition, newWidth, newHeight, items);

    EXPECT_THROW(command.DoExecute(), std::invalid_argument);
}

TEST_F(ResizeImageCommandTest, Execute_NotImage)
{
    auto paragraph = std::shared_ptr<IParagraph>(nullptr);
    std::vector<CDocumentItem> paragraphItems;
    paragraphItems.push_back(CDocumentItem(paragraph));

    ResizeImageCommand command(validPosition, newWidth, newHeight, paragraphItems);

    EXPECT_THROW(command.DoExecute(), std::invalid_argument);
}

TEST_F(ResizeImageCommandTest, MergeWith_Success)
{
    ResizeImageCommand command1(validPosition, 200, 150, items);
    ResizeImageCommand command2(validPosition, 300, 200, items);

    bool merged = command1.MergeWith(command2);
    EXPECT_TRUE(merged);
}

TEST_F(ResizeImageCommandTest, MergeWith_DifferentPositions)
{
    ResizeImageCommand command1(validPosition, 200, 150, items);
    ResizeImageCommand command2(1, 300, 200, items);

    bool merged = command1.MergeWith(command2);
    EXPECT_FALSE(merged);
}

TEST_F(ResizeImageCommandTest, MergeWith_DifferentItemsVectors)
{
    std::vector<CDocumentItem> otherItems;
    ResizeImageCommand command1(validPosition, 200, 150, items);
    ResizeImageCommand command2(validPosition, 300, 200, otherItems);

    bool merged = command1.MergeWith(command2);
    EXPECT_FALSE(merged);
}

TEST_F(ResizeImageCommandTest, MergeWith_MultipleCommands)
{
    ResizeImageCommand command1(validPosition, 200, 150, items);
    ResizeImageCommand command2(validPosition, 300, 200, items);
    ResizeImageCommand command3(validPosition, 400, 300, items);

    bool merged1 = command1.MergeWith(command2);
    EXPECT_TRUE(merged1);

    bool merged2 = command1.MergeWith(command3);
    EXPECT_TRUE(merged2);
}

TEST_F(ResizeImageCommandTest, ExecuteAfterMerge_SetsFinalSize)
{
    ResizeImageCommand command1(validPosition, 200, 150, items);
    ResizeImageCommand command2(validPosition, 300, 200, items);

    bool merged = command1.MergeWith(command2);
    EXPECT_TRUE(merged);

    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, Resize(300, 200)).Times(1);

    command1.DoExecute();
}

TEST_F(ResizeImageCommandTest, UnexecuteAfterMerge_RestoresOriginalSize)
{
    ResizeImageCommand command1(validPosition, 200, 150, items);
    ResizeImageCommand command2(validPosition, 300, 200, items);

    bool merged = command1.MergeWith(command2);
    EXPECT_TRUE(merged);

    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, Resize(300, 200)).Times(1);

    EXPECT_CALL(*mockImage, Resize(100, 100)).Times(1);

    command1.DoExecute();
    command1.DoUnexecute();
}

TEST_F(ResizeImageCommandTest, Execute_ZeroSize)
{
    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, Resize(0, 0)).Times(1);

    ResizeImageCommand command(validPosition, 0, 0, items);
    EXPECT_NO_THROW(command.DoExecute());
}

TEST_F(ResizeImageCommandTest, Execute_MinimalSize)
{
    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, Resize(1, 1)).Times(1);

    ResizeImageCommand command(validPosition, 1, 1, items);
    EXPECT_NO_THROW(command.DoExecute());
}

TEST_F(ResizeImageCommandTest, CommandHistoryScenario_Simple)
{
    auto cmd1 = std::make_unique<ResizeImageCommand>(validPosition, 200, 150, items);

    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, Resize(200, 150)).Times(1);

    cmd1->DoExecute();

    auto cmd2 = std::make_unique<ResizeImageCommand>(validPosition, 300, 200, items);
    bool merged = cmd1->MergeWith(*cmd2);
    EXPECT_TRUE(merged);
}

TEST_F(ResizeImageCommandTest, MergeWith_DifferentImages)
{
    auto mockImage2 = std::make_shared<MockImage>();
    std::vector<CDocumentItem> itemsWithTwoImages;
    itemsWithTwoImages.push_back(CDocumentItem(mockImage));
    itemsWithTwoImages.push_back(CDocumentItem(mockImage2));

    ResizeImageCommand command1(0, 200, 150, itemsWithTwoImages);
    ResizeImageCommand command2(1, 300, 200, itemsWithTwoImages);

    bool merged = command1.MergeWith(command2);
    EXPECT_FALSE(merged);
}

TEST_F(ResizeImageCommandTest, PreservesCorrectOriginalSize)
{
    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(150));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(120));
    EXPECT_CALL(*mockImage, Resize(200, 150)).Times(1);
    EXPECT_CALL(*mockImage, Resize(150, 120)).Times(1);

    ResizeImageCommand command(validPosition, 200, 150, items);
    command.DoExecute();
    command.DoUnexecute();
}

TEST_F(ResizeImageCommandTest, MultipleExecuteUnexecute)
{
    ResizeImageCommand command(validPosition, 200, 150, items);
    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, Resize(200, 150)).Times(1);
    EXPECT_CALL(*mockImage, Resize(100, 100)).Times(1);

    command.DoExecute();
    command.DoUnexecute();

    EXPECT_CALL(*mockImage, GetWidth()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, GetHeight()).WillOnce(testing::Return(100));
    EXPECT_CALL(*mockImage, Resize(200, 150)).Times(1);
    EXPECT_CALL(*mockImage, Resize(100, 100)).Times(1);

    command.DoExecute();
    command.DoUnexecute();
}