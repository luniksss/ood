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
        std::cout << defineStation(subject) << std::endl;
        std::cout << "Current Temp " << data.temperature << std::endl;
        std::cout << "Current Hum " << data.humidity << std::endl;
        std::cout << "Current Pressure " << data.pressure << std::endl;
        std::cout << "----------------" << std::endl;
    }
};

#endif //CDISPLAY_H
