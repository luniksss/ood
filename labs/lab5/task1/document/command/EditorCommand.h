#ifndef EDITORCOMMAND_H
#define EDITORCOMMAND_H
#include "ICommand.h"

class EditorCommand: public ICommand
{
public:
  void Execute() override
  {
    if (!m_executed)
    {
      DoExecute();
      m_executed = true;
    }
  }

  void Unexecute() override
  {
    if (m_executed)
    {
      DoUnexecute();
      m_executed = false;
    }
  }

protected:
  virtual void DoExecute() = 0;
  virtual void DoUnexecute() = 0;

  bool IsExecuted() const
  {
    return m_executed;
  }

private:
  bool m_executed = false;
};
#endif //EDITORCOMMAND_H
