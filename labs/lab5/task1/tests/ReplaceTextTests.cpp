#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../document/command/ReplaceTextCommand.h"

class MockParagraph : public IParagraph
{
public:
    MOCK_METHOD(std::string, GetText, (), (const, override));
    MOCK_METHOD(void, SetText, (const std::string&), (override));
};

class ReplaceTextCommandTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mockParagraph = std::make_shared<MockParagraph>();
        items.push_back(CDocumentItem(mockParagraph));
    }

    std::shared_ptr<MockParagraph> mockParagraph;
    std::vector<CDocumentItem> items;
    const std::string initialText = "Initial text";
    const std::string newText = "New text";
    const size_t validPosition = 0;
};

TEST_F(ReplaceTextCommandTest, DoExecute_Success)
{
    EXPECT_CALL(*mockParagraph, GetText()).WillOnce(testing::Return(initialText));
    EXPECT_CALL(*mockParagraph, SetText(newText)).Times(1);

    ReplaceTextCommand command(newText, validPosition, items);
    command.DoExecute();
}

TEST_F(ReplaceTextCommandTest, DoUnexecute_Success)
{
    testing::InSequence seq;

    EXPECT_CALL(*mockParagraph, GetText()).WillOnce(testing::Return(initialText));
    EXPECT_CALL(*mockParagraph, SetText(newText)).Times(1);

    EXPECT_CALL(*mockParagraph, GetText()).WillOnce(testing::Return(newText));
    EXPECT_CALL(*mockParagraph, SetText(initialText)).Times(1);

    ReplaceTextCommand command(newText, validPosition, items);
    command.DoExecute();
    command.DoUnexecute();
}

TEST_F(ReplaceTextCommandTest, DoExecute_OutOfRange)
{
    constexpr size_t invalidPosition = 1;
    ReplaceTextCommand command(newText, invalidPosition, items);

    EXPECT_THROW(command.DoExecute(), std::invalid_argument);
}

TEST_F(ReplaceTextCommandTest, DoExecute_NotParagraph_WithNullptr)
{
    std::vector<CDocumentItem> emptyItems;
    auto tempParagraph = std::shared_ptr<IParagraph>(nullptr);
    emptyItems.push_back(CDocumentItem(tempParagraph));

    ReplaceTextCommand command(newText, 0, emptyItems);

    EXPECT_THROW(command.DoExecute(), std::invalid_argument);
}

TEST_F(ReplaceTextCommandTest, TextIsCorrectlySavedAndRestored)
{
    testing::Sequence seq;
    EXPECT_CALL(*mockParagraph, GetText()).InSequence(seq).WillOnce(testing::Return(initialText));
    EXPECT_CALL(*mockParagraph, SetText(newText)).InSequence(seq);
    EXPECT_CALL(*mockParagraph, GetText()).InSequence(seq).WillOnce(testing::Return(newText));
    EXPECT_CALL(*mockParagraph, SetText(initialText)).InSequence(seq);

    ReplaceTextCommand command(newText, validPosition, items);

    EXPECT_NO_THROW(command.DoExecute());
    EXPECT_NO_THROW(command.DoUnexecute());
}

TEST_F(ReplaceTextCommandTest, HandlesEmptyText)
{
    const std::string emptyText;

    EXPECT_CALL(*mockParagraph, GetText()).WillOnce(testing::Return(initialText));
    EXPECT_CALL(*mockParagraph, SetText(emptyText)).Times(1);

    ReplaceTextCommand command(emptyText, validPosition, items);
    EXPECT_NO_THROW(command.DoExecute());
}

TEST_F(ReplaceTextCommandTest, MultipleExecuteUnexecute)
{
    const std::string anotherText = "Another text";

    testing::Sequence seq;
    EXPECT_CALL(*mockParagraph, GetText()).InSequence(seq).WillOnce(testing::Return(initialText));
    EXPECT_CALL(*mockParagraph, SetText(newText)).InSequence(seq);
    EXPECT_CALL(*mockParagraph, GetText()).InSequence(seq).WillOnce(testing::Return(newText));
    EXPECT_CALL(*mockParagraph, SetText(initialText)).InSequence(seq);
    EXPECT_CALL(*mockParagraph, GetText()).InSequence(seq).WillOnce(testing::Return(initialText));
    EXPECT_CALL(*mockParagraph, SetText(anotherText)).InSequence(seq);

    ReplaceTextCommand command(newText, validPosition, items);

    command.DoExecute();
    command.DoUnexecute();
    ReplaceTextCommand anotherCommand(anotherText, validPosition, items);
    anotherCommand.DoExecute();
}