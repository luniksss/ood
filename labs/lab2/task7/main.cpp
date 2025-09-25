#include "Animals/CAnimalData.h"
#include "Animals/CCatObserver.h"
#include "Animals/CDogObserver.h"
#include "Animals/CMouseObserver.h"
#include "./WeatherStation/Observable/CWeatherData.h"
#include "./WeatherStation/Observer/CFishermanDisplay.h"

void ProcessAnimals()
{
    CAnimalData ani;
    CCatObserver cat;
    CDogObserver dog;
    CMouseObserver mouse;

    ani.RegisterObserver(cat, 2, EAnimalEventType::Sound);
    ani.RegisterObserver(cat, 2, EAnimalEventType::Move);
    ani.RegisterObserver(mouse, 1, EAnimalEventType::Move);

    const SAnimalInfo animalInfo = {"mouse"};
    ani.SetMeasurements(EAnimalEventType::Move, animalInfo);
}

void ProcessWeather()
{
    CWeatherData wd;

    CFishermanDisplay display;
    wd.RegisterObserver(display, 1, EWeatherEventType::Temperature);
    wd.RegisterObserver(display, 1, EWeatherEventType::Pressure);

    constexpr SWindInfo wind = {3, 10};
    wd.SetMeasurements(10, 0.8, 761, wind);
    wd.SetMeasurements(-10, 0.8, 761, wind);

    wd.RegisterObserver(display, 1, EWeatherEventType::Humidity);
    wd.SetHumidity(0.7);
}

int main()
{
    ProcessAnimals();
    ProcessWeather();
    return 0;
}