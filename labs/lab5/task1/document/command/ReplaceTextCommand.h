#ifndef REPLACETEXTCOMMAND_H
#define REPLACETEXTCOMMAND_H
#include "EditorCommand.h"
#include <string>
#include <utility>
#include <vector>
#include "../CDocumentItem.h"

class ReplaceTextCommand: public EditorCommand
{
public:
    ReplaceTextCommand(
        std::string newText,
        const size_t position,
        std::vector<CDocumentItem> &items
    ): m_newText(std::move(newText)),
       m_position(position),
       m_items(items)
    {
    }

    void DoExecute() override
    {
        std::shared_ptr<IParagraph> paragraph = GetParagraph();
        m_previousText = paragraph->GetText();
        paragraph->SetText(m_newText);
    }

    void DoUnexecute() override
    {
        std::shared_ptr<IParagraph> paragraph = GetParagraph();
        m_newText = paragraph->GetText();
        paragraph->SetText(m_previousText);
    }

private:
    std::string m_previousText{};
    std::string m_newText;
    size_t m_position;
    std::vector<CDocumentItem> &m_items;

    void CheckRange() const
    {
        if (m_position >= m_items.size())
        {
            throw std::invalid_argument("Out of range");
        }
    }

    static void CheckParagraph(const std::shared_ptr<IParagraph>& paragraph)
    {
        if (paragraph == nullptr)
        {
            throw std::invalid_argument("This item is not a text");
        }
    }

    std::shared_ptr<IParagraph> GetParagraph()
    {
        CheckRange();
        auto item = m_items.at(m_position);
        const auto paragraph = item.GetParagraph();
        CheckParagraph(paragraph);
        return paragraph;
    }
};

#endif //REPLACETEXTCOMMAND_H
