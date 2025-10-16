#ifndef SETTITLECOMMAND_H
#define SETTITLECOMMAND_H
#include "EditorCommand.h"
#include <string>

class SetTitleCommand : public EditorCommand
{
public:
    explicit SetTitleCommand(
        std::string &title,
        std::string newTitle
    ): m_title(title),
       m_newTitle(std::move(newTitle))
    {
    }

    void DoExecute() override
    {
//        std::string temp = std::move(m_title);
//        m_title = std::move(m_newTitle);
//        m_newTitle = std::move(temp);
        std::swap(m_title, m_newTitle);
    }

    void DoUnexecute() override
    {
        std::swap(m_newTitle, m_title);
    }

    bool MergeWith(const SetTitleCommand &other)
    {
        if (&m_title != &other.m_title)
        {
          return false;
        }

        m_newTitle = other.m_newTitle;
        return true;
    }

private:
    std::string &m_title;
    std::string m_newTitle;
};

#endif //SETTITLECOMMAND_H
