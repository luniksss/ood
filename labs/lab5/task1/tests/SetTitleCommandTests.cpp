#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../document/command/SetTitleCommand.h"

class Document
{
public:
    void SetTitle(const std::string& title)
    {
        m_title = title;
    }
    std::string GetTitle() const
    {
        return m_title;
    }

    std::string& GetTitleRef()
    {
        return m_title;
    }
private:
    std::string m_title;
};

class SetTitleCommandTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        doc.SetTitle("Old Title");
    }

    Document doc;
};

TEST_F(SetTitleCommandTests, ExecuteChangesTitle)
{
    SetTitleCommand cmd(doc.GetTitleRef(), "New Title");
    cmd.Execute();
    EXPECT_EQ(doc.GetTitle(), "New Title");
}

TEST_F(SetTitleCommandTests, UndoRestoresOldTitle)
{
    SetTitleCommand cmd(doc.GetTitleRef(), "New Title");
    cmd.Execute();
    EXPECT_EQ(doc.GetTitle(), "New Title");

    cmd.Unexecute();
    EXPECT_EQ(doc.GetTitle(), "Old Title");
}

TEST_F(SetTitleCommandTests, MultipleExecuteUnexecuteCycles)
{
    SetTitleCommand cmd(doc.GetTitleRef(), "New Title");

    cmd.Execute();
    EXPECT_EQ(doc.GetTitle(), "New Title");
    cmd.Unexecute();
    EXPECT_EQ(doc.GetTitle(), "Old Title");

    cmd.Execute();
    EXPECT_EQ(doc.GetTitle(), "New Title");
    cmd.Unexecute();
    EXPECT_EQ(doc.GetTitle(), "Old Title");
}

TEST_F(SetTitleCommandTests, MergeWithDifferentTargetFails)
{
    std::string otherTitle = "Other Title";

    SetTitleCommand firstCmd(doc.GetTitleRef(), "Title1");
    SetTitleCommand secondCmd(otherTitle, "Title2");

    firstCmd.Execute();
    EXPECT_EQ(doc.GetTitle(), "Title1");

    bool merged = firstCmd.MergeWith(secondCmd);
    EXPECT_FALSE(merged);
}

TEST_F(SetTitleCommandTests, EmptyTitleWorks)
{
    SetTitleCommand cmd(doc.GetTitleRef(), "");
    cmd.Execute();
    EXPECT_EQ(doc.GetTitle(), "");

    cmd.Unexecute();
    EXPECT_EQ(doc.GetTitle(), "Old Title");
}