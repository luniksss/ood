#ifndef CSTATSDISPLAY_H
#define CSTATSDISPLAY_H
#include "../SWeatherInfo.h"
#include "../IObserver.h"
#include "./CCounter.h"
#include "./CWindCounter.h"
#include <iostream>

class CStatsDisplay : public IObserver<SWeatherInfo>
{
public:
    CStatsDisplay(
        IObservable<SWeatherInfo> *stationIn,
        const int stationInPriority,
        IObservable<SWeatherInfo> *stationOut,
        const int stationOutPriority
    ): m_stationIn(stationIn), m_stationOut(stationOut)
    {
        //TODO подписываться
        if (m_stationIn)
        {
            m_stationIn->RegisterObserver(*this, stationInPriority);
        }
        if (m_stationOut)
        {
            m_stationOut->RegisterObserver(*this, stationOutPriority);
        }
    }

    //TODO отписываться в деструкторе
    ~CStatsDisplay() override
    {
        if (m_stationIn)
        {
            m_stationIn->RemoveObserver(*this);
        }
        if (m_stationOut)
        {
            m_stationOut->RemoveObserver(*this);
        }
    }

private:
    IObservable<SWeatherInfo> *m_stationIn{};
    IObservable<SWeatherInfo> *m_stationOut{};

    void calculateData(SWeatherInfo const& data)
    {
        m_temperature.ProcessData(data.temperature);
        m_humidity.ProcessData(data.humidity);
        m_pressure.ProcessData(data.pressure);
        m_wind.ProcessData(data.wind);
    }

    static void DisplayData(const CCounter &data, const std::string& name)
    {
        std::cout << "Max " << name << " " << data.GetMaxData() << std::endl;
        std::cout << "Min " << name << " " << data.GetMinData() << std::endl;
        std::cout << "Average " << name << " " << data.GetAverage() << std::endl;
    }

    static void DisplayWindData(const CWindCounter &data, const std::string& name)
    {
        std::cout << "Max " << name << " " << data.GetMaxWindSpeed() << std::endl;
        std::cout << "Min " << name << " " << data.GetMinWindSpeed() << std::endl;
        std::cout << "Average " << name << " " << data.GetWindSpeedAverage() << std::endl;
        std::cout << "Average " << name << " direction " << data.GetWindDirectionAverage() << " degrees" << std::endl;
    }

    //TODO лучше возвращать true/false
    bool DefineStation(const IObservable<SWeatherInfo>* subject) const
    {
        if (subject == m_stationIn)
        {
            std::cout << "Inside station" << std::endl;
            return true;
        }
        if (subject == m_stationOut)
        {
            std::cout << "Outside station" << std::endl;
            DisplayWindData(m_wind, "Wind");
            return true;
        }
        std::cout << "Undefined station" << std::endl;
        return false;
    }

    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SWeatherInfo const& data, const IObservable<SWeatherInfo>* subject) override
    {
        calculateData(data);

        if (!DefineStation(subject))
        {
            return;
        };
        DisplayData(m_temperature, "Temperature");
        DisplayData(m_humidity, "Humidity");
        DisplayData(m_pressure, "Pressure");
        std::cout << "----------------" << std::endl;
    }

    CCounter m_temperature;
    CCounter m_humidity;
    CCounter m_pressure;
    CWindCounter m_wind;
};

#endif //CSTATSDISPLAY_H
