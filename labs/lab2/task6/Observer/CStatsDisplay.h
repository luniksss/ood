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
        IObservable<SWeatherInfo> *stationOut
    ): m_stationIn(stationIn), m_stationOut(stationOut)
    {
    };
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

    int defineStation(const IObservable<SWeatherInfo>* subject) const
    {
        if (subject == m_stationIn)
        {
            std::cout << "Inside station" << std::endl;
            return 0;
        }
        if (subject == m_stationOut)
        {
            std::cout << "Outside station" << std::endl;
            m_wind.DisplayData("Wind");
            return 0;
        }
        std::cout << "Undefined station" << std::endl;
        return 1;
    }

    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SWeatherInfo const& data, const IObservable<SWeatherInfo>* subject) override
    {
        calculateData(data);

        if (defineStation(subject))
        {
            return;
        };
        m_temperature.DisplayData("Temperature");
        m_humidity.DisplayData("Humidity");
        m_pressure.DisplayData("Pressure");
        std::cout << "----------------" << std::endl;
    }

    CCounter m_temperature;
    CCounter m_humidity;
    CCounter m_pressure;
    CWindCounter m_wind;
};

#endif //CSTATSDISPLAY_H
