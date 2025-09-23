#ifndef COBSERVABLE_H
#define COBSERVABLE_H
#include "IObservable.h"
#include <set>
#include <vector>
#include <map>
#include <unordered_map>

template <class T>
class CObservable : public IObservable<T>
{
public:
    typedef IObserver<T> ObserverType;

    void RegisterObserver(ObserverType & observer, int priority) override
    {
        //TODO сделать последовательный обход(наблюдатели - ключи)
        if (m_observerPriorityMap.contains(&observer))
        {
            return;
        }
        m_observers[priority].insert(&observer);
        m_observerPriorityMap[&observer] = priority;
    }

    void NotifyObservers() override
    {
        T data = GetChangedData();
        //TODO сделать копию до цикла
        std::vector<ObserverType*> observersCopy;
        for (const auto& [priority, observersSet] : m_observers)
        {
            observersCopy.insert(observersCopy.end(), observersSet.begin(), observersSet.end());
        }

        for (auto observer : observersCopy)
        {
            observer->Update(data);
        }
    }

    void RemoveObserver(ObserverType & observer) override
    {
        auto observerPriority = m_observerPriorityMap.find(&observer);
        if (observerPriority == m_observerPriorityMap.end())
        {
            return;
        }

        int priority = observerPriority->second;
        auto requiredObserver = m_observers.find(priority);
        if (requiredObserver != m_observers.end())
        {
            requiredObserver->second.erase(&observer);
            if (requiredObserver->second.empty())
            {
                m_observers.erase(requiredObserver);
            }
        }
        m_observerPriorityMap.erase(observerPriority);
    }

protected:
    // Классы-наследники должны перегрузить данный метод,
    // в котором возвращать информацию об изменениях в объекте
    virtual T GetChangedData() const = 0;

private:
    std::map<int, std::set<ObserverType*>, std::greater<int>> m_observers;
    std::unordered_map<ObserverType*, int> m_observerPriorityMap;
};

#endif //COBSERVABLE_H
