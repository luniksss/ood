#ifndef CDOGOBSERVER_H
#define CDOGOBSERVER_H
#include "./SAnimalInfo.h"
#include "./EAnimalEventType.h"
#include "../WeatherStation/Observer/IObserver.h"
#include <iostream>

class CDogObserver : public IObserver<SAnimalInfo, EAnimalEventType>
{
private:
    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SAnimalInfo const& data, const EAnimalEventType eventType) override {
        if (data.name == "dog")
        {
            switch (eventType)
            {
                case EAnimalEventType::Sound:
                    std::cout << "Woof! I'm a " << data.name << std::endl;
                break;
                case EAnimalEventType::Move:
                    std::cout << "I move like a hurricane, cause i'm a " << data.name << std::endl;
                break;
            }
            std::cout << "----------------" << std::endl;
        }
    }
};

#endif //CDOGOBSERVER_H
