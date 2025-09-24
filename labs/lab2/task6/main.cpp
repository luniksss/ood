#include "./Observable/CWeatherData.h"
#include "./Observer/CDisplay.h"
#include "./Observer/CStatsDisplay.h"

int main()
{
    CWeatherData weatherIn;
    CWeatherData weatherOut;
    SWindInfo wind = {3, 10};

    //TODO лучше регистрироваться самому через конструктор и отписываться через деструктор
    CDisplay display(&weatherIn, 1, &weatherOut, 1);
    CStatsDisplay statsDisplay(&weatherIn, 2, &weatherOut, 2);

    weatherIn.SetMeasurements(3, 0.7, 760, wind);
    weatherOut.SetMeasurements(15, 0.55, 750, wind);

    weatherIn.SetMeasurements(4, 0.8, 761, wind);
    weatherOut.SetMeasurements(16, 0.60, 751, wind);

    wind.direction = 100;
    wind.speed = 10;
    weatherIn.SetMeasurements(10, 0.8, 761, wind);
    weatherOut.SetMeasurements(-10, 0.9, 740, wind);

    return 0;
}