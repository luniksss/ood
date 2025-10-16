#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../document/History.h"

class MockCommand : public ICommand
{
public:
    MOCK_METHOD(void, Execute, (), (override));
    MOCK_METHOD(void, Unexecute, (), (override));
};

class HistoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mockCommand1 = std::make_unique<MockCommand>();
        mockCommand2 = std::make_unique<MockCommand>();
        mockCommand3 = std::make_unique<MockCommand>();

        mockCommand1Ptr = mockCommand1.get();
        mockCommand2Ptr = mockCommand2.get();
        mockCommand3Ptr = mockCommand3.get();
    }

    std::unique_ptr<MockCommand> mockCommand1;
    std::unique_ptr<MockCommand> mockCommand2;
    std::unique_ptr<MockCommand> mockCommand3;

    MockCommand*mockCommand1Ptr = nullptr;
    MockCommand*mockCommand2Ptr = nullptr;
    MockCommand*mockCommand3Ptr = nullptr;

    History history;
};

TEST_F(HistoryTest, CanUndoInitiallyFalse)
{
    EXPECT_FALSE(history.CanUndo());
}

TEST_F(HistoryTest, CanRedoInitiallyFalse)
{
    EXPECT_FALSE(history.CanRedo());
}

TEST_F(HistoryTest, AddAndExecuteCommandExecutesCommand)
{
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));
}

TEST_F(HistoryTest, AddAndExecuteCommandCanUndoBecomesTrue)
{
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));

    EXPECT_TRUE(history.CanUndo());
}

TEST_F(HistoryTest, UndoAfterAddCommandUnexecutesCommand)
{
    testing::InSequence seq;
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Unexecute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));
    history.Undo();
}

TEST_F(HistoryTest, UndoAfterAddCommandCanUndoBecomesFalse)
{
    testing::InSequence seq;
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Unexecute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));

    EXPECT_TRUE(history.CanUndo());

    history.Undo();

    EXPECT_FALSE(history.CanUndo());
}

TEST_F(HistoryTest, UndoAfterAddCommandCanRedoBecomesTrue)
{
    testing::InSequence seq;
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Unexecute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));
    history.Undo();

    EXPECT_TRUE(history.CanRedo());
}

TEST_F(HistoryTest, RedoAfterUndoExecutesCommand)
{
    testing::InSequence seq;
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));
    history.Undo();
    history.Redo();
}

TEST_F(HistoryTest, RedoAfterUndoCanRedoBecomesFalse)
{
    testing::InSequence seq;
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));
    history.Undo();

    EXPECT_TRUE(history.CanRedo());

    history.Redo();

    EXPECT_FALSE(history.CanRedo());
}

TEST_F(HistoryTest, MultipleCommandsUndoRedoSequence)
{
    testing::InSequence seq;
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Execute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));
    history.AddAndExecuteCommand(std::move(mockCommand2));

    EXPECT_TRUE(history.CanUndo());

    history.Undo();
    history.Undo();

    EXPECT_FALSE(history.CanUndo());
    EXPECT_TRUE(history.CanRedo());

    history.Redo();
    history.Redo();

    EXPECT_FALSE(history.CanRedo());
}

TEST_F(HistoryTest, AddCommandAfterUndoClearsRedoStack)
{
    testing::InSequence seq;
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand3Ptr, Execute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));
    history.AddAndExecuteCommand(std::move(mockCommand2));

    history.Undo();

    EXPECT_TRUE(history.CanRedo());

    history.AddAndExecuteCommand(std::move(mockCommand3));

    EXPECT_FALSE(history.CanRedo());
    EXPECT_TRUE(history.CanUndo());
}

TEST_F(HistoryTest, UndoWhenCannotUndoDoesNothing)
{
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(0);
    EXPECT_CALL(*mockCommand1Ptr, Unexecute()).Times(0);

    history.Undo();
}

TEST_F(HistoryTest, RedoWhenCannotRedoDoesNothing)
{
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(0);
    EXPECT_CALL(*mockCommand1Ptr, Unexecute()).Times(0);

    history.Redo();
}

TEST_F(HistoryTest, HistoryLimitRespectsMaxSize)
{
    History history;

    for (int i = 0; i < 12; ++i) {
        auto command = std::make_unique<MockCommand>();
        auto* commandPtr = command.get();

        EXPECT_CALL(*commandPtr, Execute()).Times(1);

        history.AddAndExecuteCommand(std::move(command));
    }

    EXPECT_TRUE(history.CanUndo());
}

TEST_F(HistoryTest, ComplexUndoRedoScenario)
{
    testing::InSequence seq;
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand3Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand3Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand3Ptr, Execute()).Times(1);

    history.AddAndExecuteCommand(std::move(mockCommand1));
    history.AddAndExecuteCommand(std::move(mockCommand2));

    history.Undo();
    history.Undo();

    history.Redo();
    history.Redo();

    history.Undo();

    history.AddAndExecuteCommand(std::move(mockCommand3));

    history.Undo();
    history.Redo();
}

TEST_F(HistoryTest, AddCommandUpdatesActionIndexCorrectly)
{
    testing::InSequence seq;
    EXPECT_CALL(*mockCommand1Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Execute()).Times(1);
    EXPECT_CALL(*mockCommand2Ptr, Unexecute()).Times(1);
    EXPECT_CALL(*mockCommand3Ptr, Execute()).Times(1);
    
    history.AddAndExecuteCommand(std::move(mockCommand1));
    history.AddAndExecuteCommand(std::move(mockCommand2));
    
    EXPECT_TRUE(history.CanUndo());
    
    history.Undo();
    
    EXPECT_TRUE(history.CanRedo());
    
    history.AddAndExecuteCommand(std::move(mockCommand3));
    
    EXPECT_TRUE(history.CanUndo());
    EXPECT_FALSE(history.CanRedo());
}