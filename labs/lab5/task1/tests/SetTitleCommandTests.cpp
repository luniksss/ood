#include <gtest/gtest.h>
#include <string>
#include <vector>

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

private:
    std::string m_title;
};

class SetTitleCommand
{
public:
    SetTitleCommand(Document& doc, const std::string& newTitle)
        : m_document(doc),
        m_newTitle(newTitle),
        m_oldTitle(doc.GetTitle())
    {}

    void Execute()
    {
        m_document.SetTitle(m_newTitle);
    }

    void Unexecute()
    {
        m_document.SetTitle(m_oldTitle);
    }

    bool MergeWith(const SetTitleCommand& other)
    {
        if (&m_document != &other.m_document) return false;
        m_newTitle = other.m_newTitle;
        return true;
    }

    std::string GetNewTitle() const
    {
        return m_newTitle;
    }

private:
    Document& m_document;
    std::string m_newTitle;
    std::string m_oldTitle;
};

class SetTitleCommandTests : public ::testing::Test
{
protected:
    Document doc;
    std::vector<SetTitleCommand> commandHistory;
};

TEST_F(SetTitleCommandTests, ExecuteChangesTitle)
{
    SetTitleCommand cmd(doc, "New Title");
    cmd.Execute();
    EXPECT_EQ(doc.GetTitle(), "New Title");
}

TEST_F(SetTitleCommandTests, UndoRestoresOldTitle)
{
    doc.SetTitle("Old Title");
    SetTitleCommand cmd(doc, "New Title");
    cmd.Execute();
    cmd.Unexecute();
    EXPECT_EQ(doc.GetTitle(), "Old Title");
}

TEST_F(SetTitleCommandTests, SequentialCommandsMerge)
{
    doc.SetTitle("Original");
    SetTitleCommand firstCmd(doc, "Title1");
    SetTitleCommand secondCmd(doc, "Title2");

    firstCmd.Execute();
    EXPECT_EQ(doc.GetTitle(), "Title1");

    bool merged = firstCmd.MergeWith(secondCmd);
    EXPECT_TRUE(merged);
    firstCmd.Execute();
    EXPECT_EQ(doc.GetTitle(), "Title2");
}

TEST_F(SetTitleCommandTests, MergeFailsIfDifferentDocuments)
{
    Document otherDoc;
    SetTitleCommand firstCmd(doc, "Title1");
    SetTitleCommand otherCmd(otherDoc, "Title2");

    bool merged = firstCmd.MergeWith(otherCmd);
    EXPECT_FALSE(merged);
}