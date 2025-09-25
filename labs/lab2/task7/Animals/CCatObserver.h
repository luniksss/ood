#ifndef CCATOBSERVER_H
#define CCATOBSERVER_H
#include "./SAnimalInfo.h"
#include "./EAnimalEventType.h"
#include "../WeatherStation/Observer/IObserver.h"
#include <iostream>

class CCatObserver : public IObserver<SAnimalInfo, EAnimalEventType>
{
private:
    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SAnimalInfo const& data, const EAnimalEventType eventType) override {
        if (data.name == "cat")
        {
            switch (eventType)
            {
                case EAnimalEventType::Sound:
                    std::cout << "Meow! I'm a " << data.name << std::endl;
                break;
                case EAnimalEventType::Move:
                    std::cout << "I move gracefully, i'm a " << data.name << std::endl;
                break;
            }
            std::cout << "----------------" << std::endl;
        }
    }
};

#endif //CCATOBSERVER_H
