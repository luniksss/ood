#ifndef CFISHERMANDISPLAY_H
#define CFISHERMANDISPLAY_H
#include "../SWeatherInfo.h"
#include "../../Observer/IObserver.h"
#include <iostream>

class CFishermanDisplay : public IObserver<SWeatherInfo, EWeatherEventType>
{
private:
    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SWeatherInfo const& data, const EWeatherEventType eventType) override
    {
        switch (eventType)
        {
            case EWeatherEventType::Temperature:
                std::cout << "Current Temp " << data.temperature << std::endl;
                break;
            case EWeatherEventType::Humidity:
                std::cout << "Current Humidity " << data.humidity << std::endl;
                break;
            case EWeatherEventType::Pressure:
                std::cout << "Current Pressure " << data.pressure << std::endl;
                break;
            case EWeatherEventType::Wind:
                std::cout << "Current wind speed " << data.wind.speed << std::endl;
                std::cout << "Current wind direction " << data.wind.direction << " degrees" << std::endl;
        }
        std::cout << "----------------" << std::endl;
    }
};

#endif //CFISHERMANDISPLAY_H
