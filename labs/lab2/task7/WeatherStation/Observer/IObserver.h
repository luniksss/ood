#ifndef IOBSERVER_H
#define IOBSERVER_H
#include "../EWeatherEventType.h"

/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T, typename EventType>
class IObserver
{
public:
    virtual void Update(T const& data, EventType eventType) = 0;
    virtual ~IObserver() = default;
};

#endif //IOBSERVER_H
