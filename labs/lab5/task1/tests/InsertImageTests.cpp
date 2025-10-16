#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "../document/command/InsertImageCommand.h"

namespace fs = std::filesystem;
class InsertImageCommandTestFixture : public ::testing::Test
{
protected:
    fs::path tempDir;
    fs::path inputFilePath;

    void SetUp() override
    {
        tempDir = fs::temp_directory_path() / "InsertImageTest";
        fs::create_directories(tempDir);
        inputFilePath = tempDir / "input.png";

        std::ofstream output(inputFilePath, std::ios::binary);
        constexpr char data[] = { static_cast<char>(0x89), 'P', 'N', 'G', '\0' };
        output.write(data, sizeof(data));
        output.close();
    }

    void TearDown() override
    {
        fs::remove_all(tempDir);
    }
};

TEST_F(InsertImageCommandTestFixture, ExecuteInsertsAtEndWhenPositionNone)
{
    std::vector<CDocumentItem> items;

    InsertImageCommand cmd(inputFilePath.string(), 100, 200, std::nullopt, items);
    cmd.Execute();

    ASSERT_EQ(items.size(), 1u);
    EXPECT_EQ(items[0].GetImage()->GetWidth(), 100u);
    EXPECT_EQ(items[0].GetImage()->GetHeight(), 200u);
}

TEST_F(InsertImageCommandTestFixture, ExecuteInsertsAtGivenPosition)
{
    std::vector<CDocumentItem> items;
    InsertImageCommand cmd1(inputFilePath.string(), 10, 10, std::nullopt, items);
    cmd1.Execute();

    InsertImageCommand cmd(inputFilePath.string(), 50, 50, 0, items);
    cmd.Execute();

    ASSERT_EQ(items.size(), 2u);
    EXPECT_EQ(items[0].GetImage()->GetWidth(), 50u);
}

TEST_F(InsertImageCommandTestFixture, ExecuteThrowsWhenPositionOutOfRange)
{
    std::vector<CDocumentItem> items;

    InsertImageCommand cmd(inputFilePath.string(), 100, 100, 1, items);
    EXPECT_THROW(cmd.Execute(), std::invalid_argument);
}

TEST_F(InsertImageCommandTestFixture, UndoOnEmptyItemsDoesNothing)
{
    std::vector<CDocumentItem> items;
    InsertImageCommand cmd(inputFilePath.string(), 10, 10, std::nullopt, items);

    cmd.Unexecute();
    EXPECT_TRUE(items.empty());
}

TEST_F(InsertImageCommandTestFixture, UndoRemovesInsertedItem)
{
    std::vector<CDocumentItem> items;
    InsertImageCommand cmd(inputFilePath.string(), 100, 100, std::nullopt, items);
    cmd.Execute();

    EXPECT_FALSE(items.empty());

    cmd.Unexecute();
    EXPECT_TRUE(items.empty());
}

TEST_F(InsertImageCommandTestFixture, ExecuteUndoExecuteRedoScenario)
{
    std::vector<CDocumentItem> items;
    InsertImageCommand cmd(inputFilePath.string(), 20, 20, std::nullopt, items);

    cmd.Execute();
    EXPECT_EQ(items.size(), 1u);

    cmd.Unexecute();
    EXPECT_TRUE(items.empty());

    cmd.Execute();
    EXPECT_EQ(items.size(), 1u);
}

TEST_F(InsertImageCommandTestFixture, DestructorDeletesCopiedFileAfterUndo)
{
    std::vector<CDocumentItem> items;
    fs::path savedPath;

    {
        InsertImageCommand cmd(inputFilePath.string(), 50, 50, std::nullopt, items);
        cmd.Execute();
        savedPath = items[0].GetImage()->GetPath();
        EXPECT_TRUE(fs::exists(savedPath));

        cmd.Unexecute();
        EXPECT_TRUE(items.empty());
    }

    EXPECT_FALSE(fs::exists(savedPath));
}

TEST_F(InsertImageCommandTestFixture, UndoErasesItemAtPositionIfPositionValid)
{
    std::vector<CDocumentItem> items;

    {
        InsertImageCommand insertCmd(inputFilePath.string(), 10, 10, std::nullopt, items);
        insertCmd.Execute();
    }
    ASSERT_EQ(items.size(), 1u);

    InsertImageCommand cmd(inputFilePath.string(), 20, 20, 0, items);
    cmd.Execute();

    ASSERT_EQ(items.size(), 2u);
    cmd.Unexecute();
    EXPECT_EQ(items.size(), 1u);
}