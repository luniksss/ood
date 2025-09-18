#ifndef ISHAPESTRATEGY_H
#define ISHAPESTRATEGY_H
#pragma once
#include <string>
#include "../../gfx/ICanvas.h"

namespace shapes
{
using Color = uint32_t;
class IShapeStrategy
{
public:
    virtual ~IShapeStrategy() = default;

    virtual void Move(double dx, double dy) = 0;
    [[nodiscard]] virtual std::string ShowInfo() const = 0;
    [[nodiscard]] virtual std::string GetShapeName() const = 0;
    virtual void Draw(gfx::ICanvas& canvas, Color color) const = 0;
};
}

#endif //ISHAPESTRATEGY_H
