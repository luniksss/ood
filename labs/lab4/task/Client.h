#ifndef CLIENT_H
#define CLIENT_H
#include "./designer/IDesigner.h"
#include "./painter/IPainter.h"

class Client
{
public:
    explicit Client(IDesigner &designer)
        : m_designer(designer)
    {
    }

    void DescribeDraft(std::istream &inputData, ICanvas &canvas, IPainter &painter) const
    {
        const PictureDraft pictureDraft = m_designer.CreateDraft(inputData);
        painter.DrawPicture(pictureDraft, canvas);
    }

private:
    IDesigner &m_designer;
};

#endif //CLIENT_H
