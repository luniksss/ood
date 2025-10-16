#ifndef HISTORY_H
#define HISTORY_H
#include <memory>
#include <vector>
#include "./command/ICommand.h"

class History
{
public:
    bool CanUndo() const
    {
        return m_actionIndex > 0;
    }

    void Undo()
    {
        if (CanUndo())
        {
            unsigned previousIndex = --m_actionIndex;
            m_commands[previousIndex]->Unexecute();
        }
    }

    bool CanRedo() const
    {
        return m_actionIndex < m_commands.size();
    }

    void Redo()
    {
        if (CanRedo())
        {
            unsigned nextIndex = m_actionIndex++;
            m_commands[nextIndex]->Execute();
        }
    }

    void AddAndExecuteCommand(std::unique_ptr<ICommand> command)
    {
        command->Execute();
        if (m_actionIndex < m_commands.size())
        {
            m_commands.erase(m_commands.begin() + m_actionIndex, m_commands.end());
        }

        m_commands.emplace_back(std::move(command));
        m_actionIndex++;

        if (m_commands.size() > MAX_HISTORY_LENGHT)
        {
            m_commands.erase(m_commands.begin());
            m_actionIndex--;
        }
    }

private:
    static constexpr unsigned MAX_HISTORY_LENGHT = 10;
    std::vector<std::unique_ptr<ICommand> > m_commands;
    unsigned m_actionIndex = 0;
};

#endif //HISTORY_H
