#ifndef RESIZEIMAGECOMMAND_H
#define RESIZEIMAGECOMMAND_H
#include "EditorCommand.h"
#include "../CDocumentItem.h"
#include <vector>

class ResizeImageCommand : public EditorCommand
{
public:
    ResizeImageCommand(
        size_t position,
        int width,
        int height,
        std::vector<CDocumentItem>& items
    ) : m_position(position),
        m_newWidth(width),
        m_newHeight(height),
        m_items(items)
    {}

    void DoExecute() override
    {
        auto image = GetImage();
        m_oldWidth = image->GetWidth();
        m_oldHeight = image->GetHeight();
        image->Resize(m_newWidth, m_newHeight);
    }

    void DoUnexecute() override
    {
        auto image = GetImage();
        image->Resize(m_oldWidth, m_oldHeight);
    }

    bool MergeWith(const ResizeImageCommand& other)
    {
        if (m_position != other.m_position || &m_items != &other.m_items)
        {
            return false;
        }

        m_newWidth = other.m_newWidth;
        m_newHeight = other.m_newHeight;
        return true;
    }

private:
    size_t m_position;
    int m_newWidth, m_newHeight;
    int m_oldWidth = 0, m_oldHeight = 0;
    std::vector<CDocumentItem>& m_items;

    void CheckRange() const
    {
        if (m_position >= m_items.size())
        {
            throw std::invalid_argument("Out of range");
        }
    }

    static void CheckImage(const std::shared_ptr<IImage>& image)
    {
        if (image == nullptr)
        {
            throw std::invalid_argument("This item is not a text");
        }
    }

    std::shared_ptr<IImage> GetImage()
    {
        CheckRange();
        auto item = m_items.at(m_position);
        auto image = item.GetImage();
        CheckImage(image);
        return image;
    }
};

#endif //RESIZEIMAGECOMMAND_H
