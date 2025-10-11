#ifndef IPAINTER_H
#define IPAINTER_H
#include "../canvas/ICanvas.h"
#include "../designer/PictureDraft.h"

class IPainter
{
public:
  virtual void DrawPicture(const PictureDraft &draft, ICanvas &canvas) = 0;
  virtual ~IPainter() {}
};

#endif //IPAINTER_H
