#ifndef CSTATSDISPLAY_H
#define CSTATSDISPLAY_H
#include "../SWeatherInfo.h"
#include "./IObserver.h"
#include "./CCounter.h"
#include "./CWindCounter.h"
#include <iostream>

class CStatsDisplay : public IObserver<SWeatherInfo>
{
private:
    void calculateData(SWeatherInfo const& data)
    {
        m_wind.ProcessData(data.wind);
        m_temperature.ProcessData(data.temperature);
        m_humidity.ProcessData(data.humidity);
        m_pressure.ProcessData(data.pressure);
    }

    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SWeatherInfo const& data) override
    {
        calculateData(data);

        m_wind.DisplayData("Wind");
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
