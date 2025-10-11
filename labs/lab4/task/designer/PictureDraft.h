#ifndef PICTUREDRAFT_H
#define PICTUREDRAFT_H
#include <memory>
#include <vector>

class PictureDraft
{
public:
    void AddShape(std::unique_ptr<shapes::Shape> shape)
    {
        m_shapes.push_back(std::move(shape));
    }

    int GetShapeCount() const
    {
        return static_cast<int>(m_shapes.size());
    }

    shapes::Shape* GetShape(const int index) const
    {
        if (index >= 0 && static_cast<std::size_t>(index) < m_shapes.size())
        {
            return m_shapes[index].get();
        }
        throw std::out_of_range("index out of range");
    }

private:
    std::vector<std::unique_ptr<shapes::Shape>> m_shapes;
};
#endif //PICTUREDRAFT_H
