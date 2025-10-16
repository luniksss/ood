#include <gtest/gtest.h>
#include "../document/command/InsertParagraphCommand.h"
#include "../document/paragraph/CParagraph.h"
#include "../document/CDocumentItem.h"

class InsertParagraphCommandTest : public ::testing::Test
{
protected:
    std::vector<CDocumentItem> items;
};

TEST_F(InsertParagraphCommandTest, InsertAtBeginning)
{
    auto par = std::make_shared<CParagraph>("old");
    items.emplace_back(par);

    InsertParagraphCommand cmd("new", 0, items);
    cmd.DoExecute();

    ASSERT_EQ(items.size(), 2u);
    EXPECT_EQ(items[0].GetParagraph()->GetText(), "new");
    EXPECT_EQ(items[1].GetParagraph()->GetText(), "old");

    cmd.DoUnexecute();
    EXPECT_EQ(items.size(), 1u);
    EXPECT_EQ(items[0].GetParagraph()->GetText(), "old");
}

TEST_F(InsertParagraphCommandTest, InsertAtMiddle)
{
    items.emplace_back(std::make_shared<CParagraph>("1"));
    items.emplace_back(std::make_shared<CParagraph>("3"));

    InsertParagraphCommand cmd("2", 1, items);
    cmd.DoExecute();

    ASSERT_EQ(items.size(), 3u);
    EXPECT_EQ(items[1].GetParagraph()->GetText(), "2");

    cmd.DoUnexecute();
    EXPECT_EQ(items.size(), 2u);
    EXPECT_EQ(items[1].GetParagraph()->GetText(), "3");
}

TEST_F(InsertParagraphCommandTest, InsertAtEndNoPosition)
{
    InsertParagraphCommand cmd("last", std::nullopt, items);
    cmd.DoExecute();

    ASSERT_EQ(items.size(), 1u);
    EXPECT_EQ(items[0].GetParagraph()->GetText(), "last");

    cmd.DoUnexecute();
    EXPECT_TRUE(items.empty());
}

TEST_F(InsertParagraphCommandTest, InsertWithInvalidPositionThrows)
{
    InsertParagraphCommand cmd("text", 5, items);
    EXPECT_THROW(cmd.DoExecute(), std::invalid_argument);
    EXPECT_TRUE(items.empty());
}

TEST_F(InsertParagraphCommandTest, InsertEmptyText)
{
    InsertParagraphCommand cmd("", std::nullopt, items);
    cmd.DoExecute();

    ASSERT_EQ(items.size(), 1u);
    EXPECT_EQ(items[0].GetParagraph()->GetText(), "");
}

TEST_F(InsertParagraphCommandTest, UndoRedoInsertParagraph)
{
    InsertParagraphCommand cmd("hello", std::nullopt, items);
    cmd.DoExecute();
    EXPECT_EQ(items.size(), 1u);

    cmd.DoUnexecute();
    EXPECT_TRUE(items.empty());

    cmd.DoExecute();
    EXPECT_EQ(items.size(), 1u);
    EXPECT_EQ(items[0].GetParagraph()->GetText(), "hello");
}
