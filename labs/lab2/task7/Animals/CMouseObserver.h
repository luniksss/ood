#ifndef CMOUSEOBSERVER_H
#define CMOUSEOBSERVER_H
#include "./SAnimalInfo.h"
#include "./EAnimalEventType.h"
#include "../Observer/IObserver.h"
#include <iostream>

class CMouseObserver : public IObserver<SAnimalInfo, EAnimalEventType>
{
private:
    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
    Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
    остается публичным
    */
    void Update(SAnimalInfo const& data, const EAnimalEventType eventType) override
    {
        if (data.name == "mouse")
        {
            switch (eventType)
            {
                case EAnimalEventType::Sound:
                    std::cout << "Squeak! I'm a " << data.name << std::endl;
                break;
                case EAnimalEventType::Move:
                    std::cout << "I move in the dark, cause i'm a " << data.name << std::endl;
                break;
            }
            std::cout << "----------------" << std::endl;
        }
    }
};

#endif //CMOUSEOBSERVER_H
