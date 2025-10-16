#include <gtest/gtest.h>
#include "../document/command/EditorCommand.h"

class TestCommand : public EditorCommand
{
public:
    int executeCount = 0;
    int unexecuteCount = 0;

protected:
    void DoExecute() override { executeCount++; }
    void DoUnexecute() override { unexecuteCount++; }
};

TEST(EditorCommandTest, ExecuteCallsDoExecuteOnce)
{
    TestCommand cmd;
    cmd.Execute();
    EXPECT_EQ(cmd.executeCount, 1);

    cmd.Execute();
    EXPECT_EQ(cmd.executeCount, 1);
}

TEST(EditorCommandTest, UnexecuteCallsDoUnexecuteOnlyIfExecuted)
{
    TestCommand cmd;
    cmd.Unexecute();
    EXPECT_EQ(cmd.unexecuteCount, 0);

    cmd.Execute();
    cmd.Unexecute();
    EXPECT_EQ(cmd.unexecuteCount, 1);

    cmd.Unexecute();
    EXPECT_EQ(cmd.unexecuteCount, 1);
}

TEST(EditorCommandTest, ExecuteAfterUnexecuteWorksAgain)
{
    TestCommand cmd;
    cmd.Execute();
    cmd.Unexecute();

    cmd.Execute();
    EXPECT_EQ(cmd.executeCount, 2);
}
