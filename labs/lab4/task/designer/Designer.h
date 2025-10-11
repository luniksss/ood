#ifndef DESIGNER_H
#define DESIGNER_H
#include "./IDesigner.h"
#include "../shapes/IShapeFactory.h"
#include <iostream>
#include <string>

class Designer: public IDesigner
{
public:
    explicit Designer(IShapeFactory &factory): m_factory(factory)
    {
    }

    PictureDraft CreateDraft(std::istream& draftData) override
    {
        PictureDraft draft;
        std::string line;
        while (std::getline(draftData, line))
        {
            if (line == "draw")
            {
                break;
            }
            if (line.empty())
            {
                continue;
            }
            draft.AddShape(m_factory.CreateShape(line));
        }

        return draft;
    }
private:
    IShapeFactory &m_factory;
};

#endif //DESIGNER_H
