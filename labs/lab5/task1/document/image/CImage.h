#ifndef CIMAGE_H
#define CIMAGE_H
#include <utility>
#include "IImage.h"

class CImage: public IImage
{
public:
    CImage(
        std::string path,
        const unsigned width,
        const unsigned height
    ): m_path(std::move(path)),
       m_width(width),
       m_height(height)
    {
    }

    std::string GetPath() const override
    {
        return m_path;
    }

    unsigned GetWidth() const override
    {
        return m_width;
    }

    unsigned GetHeight() const override
    {
        return m_height;
    }

    void Resize(const unsigned width, const unsigned height) override
    {
        m_width = width;
        m_height = height;
    }

private:
    std::string m_path;
    unsigned m_width;
    unsigned m_height;
};


#endif //CIMAGE_H
