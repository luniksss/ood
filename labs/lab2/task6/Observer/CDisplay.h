#ifndef CDISPLAY_H
#define CDISPLAY_H
#include "../SWeatherInfo.h"
#include "../IObserver.h"
#include <iostream>

class CDisplay: public IObserver<SWeatherInfo>
{
public:
    CDisplay(
        IObservable<SWeatherInfo>* stationIn,
        IObservable<SWeatherInfo>* stationOut
    ): m_stationIn(stationIn), m_stationOut(stationOut)
    {
    }
private:
    IObservable<SWeatherInfo> *m_stationIn;
    IObservable<SWeatherInfo> *m_stationOut;

    int defineStation(const IObservable<SWeatherInfo>* subject, SWeatherInfo const& data) const
    {
        if (subject == m_stationIn)
        {
            std::cout <<  "Inside station" << std::endl;
            return 0;
        }
        if (subject == m_stationOut)
        {
            std::cout <<  "Outside station" << std::endl;
            std::cout <<  "Current wind speed " << data.wind.speed << std::endl;
            std::cout <<  "Current wind direction " << data.wind.direction << std::endl;
            return 0;
        }
        std::cout <<  "Undefined station" << std::endl;
        return 1;
    }

    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
        Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
        остается публичным
    */
    void Update(SWeatherInfo const& data, const IObservable<SWeatherInfo>* subject) override
    {
        if (defineStation(subject, data))
        {
            return;
        };
        std::cout << "Current Temp " << data.temperature << std::endl;
        std::cout << "Current Hum " << data.humidity << std::endl;
        std::cout << "Current Pressure " << data.pressure << std::endl;
        std::cout << "----------------" << std::endl;
    }
};

#endif //CDISPLAY_H
