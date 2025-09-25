#ifndef CANIMALDATA_H
#define CANIMALDATA_H
#include "./SAnimalInfo.h"
#include "./EAnimalEventType.h"
#include "../Observer/CObservable.h"

class CAnimalData: public CObservable<SAnimalInfo, EAnimalEventType>
{
public:
    [[nodiscard]] std::string GetName() const
    {
        return m_name;
    }

    void MeasurementsChanged(const EAnimalEventType eventType)
    {
        NotifyObservers(eventType);
    }

    void SetMeasurements(const EAnimalEventType eventType, const SAnimalInfo& info)
    {
        m_name = info.name;
        MeasurementsChanged(eventType);
    }
protected:
    [[nodiscard]] SAnimalInfo GetChangedData() const override
    {
        SAnimalInfo info;
        info.name = GetName();
        return info;
    }
private:
    std::string m_name;
};

#endif //CANIMALDATA_H
