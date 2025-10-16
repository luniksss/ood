#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../document/command/DeleteItemCommand.h"

class MockParagraph : public IParagraph {
public:
    MOCK_METHOD(std::string, GetText, (), (const, override));
    MOCK_METHOD(void, SetText, (const std::string&), (override));
};

class MockImage : public IImage {
public:
    MOCK_METHOD(std::string, GetPath, (), (const, override));
    MOCK_METHOD(unsigned, GetWidth, (), (const, override));
    MOCK_METHOD(unsigned, GetHeight, (), (const, override));
    MOCK_METHOD(void, Resize, (unsigned width, unsigned height), (override));
};

class DeleteItemCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockParagraph = std::make_shared<MockParagraph>();
        mockImage = std::make_shared<MockImage>();

        items.push_back(CDocumentItem(mockParagraph));
        items.push_back(CDocumentItem(mockImage));
        items.push_back(CDocumentItem(mockParagraph));
    }

    std::shared_ptr<MockParagraph> mockParagraph;
    std::shared_ptr<MockImage> mockImage;
    std::vector<CDocumentItem> items;
    const size_t validPosition = 1;
    const size_t invalidPosition = 5;
};

TEST_F(DeleteItemCommandTest, Execute_Success) {
    const size_t initialSize = items.size();

    DeleteItemCommand command(validPosition, items);
    EXPECT_NO_THROW(command.DoExecute());

    EXPECT_EQ(items.size(), initialSize - 1);
}

TEST_F(DeleteItemCommandTest, Unexecute_Success) {
    const size_t initialSize = items.size();
    CDocumentItem itemToDelete = items[validPosition];

    DeleteItemCommand command(validPosition, items);

    command.DoExecute();
    EXPECT_EQ(items.size(), initialSize - 1);

    command.DoUnexecute();
    EXPECT_EQ(items.size(), initialSize);

    EXPECT_EQ(items[validPosition].GetParagraph(), itemToDelete.GetParagraph());
    EXPECT_EQ(items[validPosition].GetImage(), itemToDelete.GetImage());
}

TEST_F(DeleteItemCommandTest, Execute_DeleteFirstElement) {
    const size_t firstPosition = 0;
    CDocumentItem firstItem = items[firstPosition];

    DeleteItemCommand command(firstPosition, items);

    command.DoExecute();
    EXPECT_NE(items[0].GetParagraph(), firstItem.GetParagraph());

    command.DoUnexecute();
    EXPECT_EQ(items[firstPosition].GetParagraph(), firstItem.GetParagraph());
}

TEST_F(DeleteItemCommandTest, Execute_DeleteLastElement) {
    const size_t lastPosition = items.size() - 1;
    CDocumentItem lastItem = items[lastPosition];

    DeleteItemCommand command(lastPosition, items);

    command.DoExecute();
    EXPECT_EQ(items.size(), lastPosition);

    command.DoUnexecute();
    EXPECT_EQ(items.size(), lastPosition + 1);
    EXPECT_EQ(items[lastPosition].GetParagraph(), lastItem.GetParagraph());
}

TEST_F(DeleteItemCommandTest, Execute_InvalidPosition) {
    const size_t initialSize = items.size();

    DeleteItemCommand command(invalidPosition, items);

    EXPECT_THROW(command.DoExecute(), std::invalid_argument);
    EXPECT_EQ(items.size(), initialSize);
}

TEST_F(DeleteItemCommandTest, Execute_EmptyVector) {
    std::vector<CDocumentItem> emptyItems;

    DeleteItemCommand command(0, emptyItems);

    EXPECT_THROW(command.DoExecute(), std::invalid_argument);
}

TEST_F(DeleteItemCommandTest, MultipleExecuteUnexecute) {
    const size_t initialSize = items.size();

    DeleteItemCommand command(validPosition, items);

    command.DoExecute();
    EXPECT_EQ(items.size(), initialSize - 1);

    command.DoUnexecute();
    EXPECT_EQ(items.size(), initialSize);

    command.DoExecute();
    EXPECT_EQ(items.size(), initialSize - 1);

    command.DoUnexecute();
    EXPECT_EQ(items.size(), initialSize);
}

TEST_F(DeleteItemCommandTest, SequentialDeletion) {
    const size_t initialSize = items.size();

    DeleteItemCommand command1(0, items);
    command1.DoExecute();
    EXPECT_EQ(items.size(), initialSize - 1);

    DeleteItemCommand command2(0, items);
    command2.DoExecute();
    EXPECT_EQ(items.size(), initialSize - 2);

    command2.DoUnexecute();
    EXPECT_EQ(items.size(), initialSize - 1);

    command1.DoUnexecute();
    EXPECT_EQ(items.size(), initialSize);
}

TEST_F(DeleteItemCommandTest, PreservesItemType) {
    auto deletedItem = items[validPosition];
    const bool wasImage = (deletedItem.GetImage() != nullptr);
    const bool wasParagraph = (deletedItem.GetParagraph() != nullptr);

    DeleteItemCommand command(validPosition, items);

    command.DoExecute();
    command.DoUnexecute();

    auto restoredItem = items[validPosition];
    EXPECT_EQ(restoredItem.GetImage() != nullptr, wasImage);
    EXPECT_EQ(restoredItem.GetParagraph() != nullptr, wasParagraph);
}

TEST_F(DeleteItemCommandTest, Execute_DeleteParagraph) {
    const size_t paragraphPosition = 0;

    DeleteItemCommand command(paragraphPosition, items);

    command.DoExecute();
    EXPECT_EQ(items.size(), 2);

    command.DoUnexecute();
    EXPECT_EQ(items.size(), 3);
    EXPECT_NE(items[paragraphPosition].GetParagraph(), nullptr);
}

TEST_F(DeleteItemCommandTest, Execute_DeleteImage) {
    const size_t imagePosition = 1;

    DeleteItemCommand command(imagePosition, items);

    command.DoExecute();
    EXPECT_EQ(items.size(), 2);

    command.DoUnexecute();
    EXPECT_EQ(items.size(), 3);
    EXPECT_NE(items[imagePosition].GetImage(), nullptr);
}

TEST_F(DeleteItemCommandTest, CommandHistoryScenario) {
    std::vector<std::unique_ptr<DeleteItemCommand>> commandHistory;
    const size_t initialSize = items.size();

    auto cmd1 = std::make_unique<DeleteItemCommand>(validPosition, items);
    cmd1->DoExecute();
    commandHistory.push_back(std::move(cmd1));
    EXPECT_EQ(items.size(), initialSize - 1);

    auto cmd2 = std::make_unique<DeleteItemCommand>(0, items);
    cmd2->DoExecute();
    commandHistory.push_back(std::move(cmd2));
    EXPECT_EQ(items.size(), initialSize - 2);

    commandHistory.back()->DoUnexecute();
    EXPECT_EQ(items.size(), initialSize - 1);

    commandHistory.front()->DoUnexecute();
    EXPECT_EQ(items.size(), initialSize);
}

TEST_F(DeleteItemCommandTest, Execute_DeleteSingleElement) {
    std::vector<CDocumentItem> singleItem;
    singleItem.push_back(CDocumentItem(mockParagraph));

    DeleteItemCommand command(0, singleItem);

    command.DoExecute();
    EXPECT_TRUE(singleItem.empty());

    command.DoUnexecute();
    EXPECT_EQ(singleItem.size(), 1);
    EXPECT_NE(singleItem[0].GetParagraph(), nullptr);
}

TEST_F(DeleteItemCommandTest, UnexecuteWithoutExecute_DoesNothing) {
    const size_t initialSize = items.size();

    DeleteItemCommand command(validPosition, items);

    command.DoUnexecute();

    EXPECT_EQ(items.size(), initialSize);
}