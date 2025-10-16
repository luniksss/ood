#ifndef DELETEITEMCOMMAND_H
#define DELETEITEMCOMMAND_H
#include "EditorCommand.h"
#include "../CDocumentItem.h"
#include <vector>

class DeleteItemCommand : public EditorCommand
{
public:
    DeleteItemCommand(
        size_t position,
        std::vector<CDocumentItem>& items
    )   : m_position(position),
          m_items(items)
    {}

    void DoExecute() override
    {
        if (m_position >= m_items.size())
        {
            throw std::invalid_argument("Invalid position");
        }

        m_deletedItem = std::make_unique<CDocumentItem>(m_items.at(m_position));
        m_items.erase(m_items.begin() + m_position);
    }

    void DoUnexecute() override
    {
        if (m_deletedItem == nullptr)
        {
            return;
        }
        m_items.insert(m_items.begin() + m_position, *m_deletedItem);
    }

private:
    size_t m_position;
    std::vector<CDocumentItem>& m_items;
    std::unique_ptr<CDocumentItem> m_deletedItem = nullptr;
};

#endif //DELETEITEMCOMMAND_H
