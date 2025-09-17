#ifndef COBSERVABLE_H
#define COBSERVABLE_H
#include "../IObserver.h"
#include <set>
#include <vector>
#include <map>

template <class T>
class CObservable : public IObservable<T>
{
public:
    typedef IObserver<T> ObserverType;

    void RegisterObserver(ObserverType & observer, int priority) override
    {
        for (const auto& [prio, observersSet] : m_observers)
        {
            if (observersSet.find(&observer) != observersSet.end())
            {
                return;
            }
        }

        m_observers[priority].insert(&observer);
    }

    void NotifyObservers() override
    {
        T data = GetChangedData();
        for (const auto& [priority, observersSet] : m_observers)
        {
            std::vector<ObserverType*> observersCopy(observersSet.begin(), observersSet.end());
            for (auto observer : observersCopy)
            {
                observer->Update(data, this);
            }
        }
    }

    void RemoveObserver(ObserverType & observer) override
    {
        for (auto it = m_observers.begin(); it != m_observers.end(); ++it)
        {
            if (it->second.erase(&observer) > 0)
            {
                if (it->second.empty())
                {
                    m_observers.erase(it);
                }
                break;
            }
        }
    }

protected:
    // Классы-наследники должны перегрузить данный метод,
    // в котором возвращать информацию об изменениях в объекте
    virtual T GetChangedData() const = 0;

private:
    std::map<int, std::set<ObserverType*>, std::greater<int>> m_observers;
};

#endif //COBSERVABLE_H
