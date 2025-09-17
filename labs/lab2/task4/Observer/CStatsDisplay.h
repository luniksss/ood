#ifndef CSTATSDISPLAY_H
#define CSTATSDISPLAY_H
#include "../SWeatherInfo.h"
#include "../IObserver.h"
#include "./CCounter.h"
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
    }

    std::string defineStation(const IObservable<SWeatherInfo>* subject) const
    {
        if (subject == m_stationIn)
        {
            return "Inside station";
        }
        if (subject == m_stationOut)
        {
            return "Outside station";
        }
        return "Undefined station";
    }

    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SWeatherInfo const& data, const IObservable<SWeatherInfo>* subject) override
    {
        calculateData(data);

        std::cout << defineStation(subject) << std::endl;
        m_temperature.DisplayData("Temperature");
        m_humidity.DisplayData("Humidity");
        m_pressure.DisplayData("Pressure");
        std::cout << "----------------" << std::endl;
    }

    CCounter m_temperature;
    CCounter m_humidity;
    CCounter m_pressure;

};

#endif //CSTATSDISPLAY_H
