#ifndef IOBSERVER_H
#define IOBSERVER_H

template<typename T>
class IObserver;

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

/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T>
class IObserver
{
public:
    virtual void Update(T const& data, const IObservable<T>* subject) = 0;
    virtual ~IObserver() = default;
};

#endif //IOBSERVER_H
