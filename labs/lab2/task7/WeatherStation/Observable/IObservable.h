#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H
#include "../Observer/IObserver.h"
/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T, typename EventType>
class IObservable
{
public:
    virtual ~IObservable() = default;
    virtual void RegisterObserver(IObserver<T, EventType> & observer, int priority, EventType eventType) = 0;
    virtual void NotifyObservers(EventType eventType) = 0;
    virtual void RemoveObserver(IObserver<T, EventType> & observer, EventType eventType) = 0;
    virtual void RemoveObserverFromAll(IObserver<T, EventType>& observer) = 0;
};

#endif //IOBSERVABLE_H
