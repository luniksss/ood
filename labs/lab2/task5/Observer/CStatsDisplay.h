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
    void CalculateData(SWeatherInfo const& data)
    {
        m_wind.ProcessData(data.wind);
        m_temperature.ProcessData(data.temperature);
        m_humidity.ProcessData(data.humidity);
        m_pressure.ProcessData(data.pressure);
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

    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SWeatherInfo const& data) override
    {
        CalculateData(data);

        DisplayWindData(m_wind, "Wind");
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
