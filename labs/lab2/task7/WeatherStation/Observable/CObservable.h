#ifndef COBSERVABLE_H
#define COBSERVABLE_H
#include "IObservable.h"
#include <set>
#include <vector>
#include <map>
#include <unordered_map>

template <class T, class EventType>
class CObservable : public IObservable<T, EventType>
{
public:
    typedef IObserver<T, EventType> ObserverType;

    void RegisterObserver(ObserverType& observer, int priority, EventType eventType) override
    {
        if (!m_observers.contains(eventType))
        {
            m_observers[eventType] = {};
        }
        if (!m_observers.at(eventType).contains(priority))
        {
            m_observers.at(eventType).insert({priority, {}});
        }
        m_observers.at(eventType).at(priority).insert(&observer);
        m_observerPriorityMap[eventType][&observer] = priority;
    }

    void NotifyObservers(EventType eventType) override
    {
        T data = GetChangedData();
        if (!m_observers.contains(eventType))
        {
            return;
        }
        auto observersCopy = m_observers.at(eventType);

        for (auto &[priority, observers]: observersCopy)
        {
            for (auto &observer: observers)
            {
                observer->Update(data, eventType);
            }
        }
    }

    void RemoveObserver(ObserverType& observer, EventType eventType) override
    {
        auto& priorityMap = m_observerPriorityMap[eventType];
        auto observerPriority = priorityMap.find(&observer);
        if (observerPriority == priorityMap.end())
        {
            return;
        }

        int priority = observerPriority->second;
        auto& observersByEvent = m_observers[eventType];
        auto requiredObserver = observersByEvent.find(priority);
        if (requiredObserver != observersByEvent.end())
        {
            requiredObserver->second.erase(&observer);
            if (requiredObserver->second.empty())
            {
                observersByEvent.erase(requiredObserver);
            }
        }
        priorityMap.erase(observerPriority);
    }

    void RemoveObserverFromAll(ObserverType& observer) override
    {
        for (auto& [eventType, priorityMap] : m_observerPriorityMap)
        {
            if (priorityMap.count(&observer) != 0)
            {
                RemoveObserver(observer, eventType);
            }
        }
    }

protected:
    // Классы-наследники должны перегрузить данный метод,
    // в котором возвращать информацию об изменениях в объекте
    virtual T GetChangedData() const = 0;

private:
    std::map<EventType, std::map<int, std::set<ObserverType*>, std::greater<int>>> m_observers;
    std::map<EventType, std::unordered_map<ObserverType*, int>> m_observerPriorityMap;
};

#endif //COBSERVABLE_H
