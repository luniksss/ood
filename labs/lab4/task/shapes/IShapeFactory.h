#ifndef ISHAPEFACTORY_H
#define ISHAPEFACTORY_H
#include "./Shape.h"
#include <string>

class IShapeFactory
{
public:
    virtual std::unique_ptr<shapes::Shape> CreateShape(std::string description) = 0;
    virtual ~IShapeFactory() = default;
};

#endif //ISHAPEFACTORY_H
