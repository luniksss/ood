#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H
#include "../Observer/IObserver.h"
/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable
{
public:
    virtual ~IObservable() = default;
    virtual void RegisterObserver(IObserver<T> & observer, int priority) = 0;
    virtual void NotifyObservers() = 0;
    virtual void RemoveObserver(IObserver<T> & observer) = 0;
};

#endif //IOBSERVABLE_H
