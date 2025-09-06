#ifndef IDANCEBEHAVIOR_H
#define IDANCEBEHAVIOR_H

struct IDanceBehavior
{
    virtual ~IDanceBehavior(){};
    virtual void Dance() = 0;
};

#endif //IDANCEBEHAVIOR_H