#ifndef CSTATSDISPLAY_H
#define CSTATSDISPLAY_H
#include "../SWeatherInfo.h"
#include "./IObserver.h"
#include "./CCounter.h"
#include <iostream>
#include <set>

class CStatsDisplay : public IObserver<SWeatherInfo>
{
private:
    void CalculateData(SWeatherInfo const& data)
    {
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


    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SWeatherInfo const& data) override
    {
        CalculateData(data);

        DisplayData(m_temperature, "Temperature");
        DisplayData(m_humidity, "Humidity");
        DisplayData(m_pressure, "Pressure");
        std::cout << "----------------" << std::endl;
    }

    CCounter m_temperature;
    CCounter m_humidity;
    CCounter m_pressure;

};

#endif //CSTATSDISPLAY_H
