#include "./Observable/CWeatherData.h"
#include "./Observer/CDisplay.h"
#include "./Observer/CStatsDisplay.h"

int main()
{
    CWeatherData weatherIn;
    CWeatherData weatherOut;

    CDisplay display(&weatherIn, &weatherOut);
    CStatsDisplay statsDisplay(&weatherIn, &weatherOut);

    weatherIn.RegisterObserver(display, 1);
    weatherOut.RegisterObserver(display, 1);
    weatherIn.RegisterObserver(statsDisplay, 2);
    weatherOut.RegisterObserver(statsDisplay, 2);

    weatherIn.SetMeasurements(3, 0.7, 760);
    weatherOut.SetMeasurements(15, 0.55, 750);

    weatherIn.SetMeasurements(4, 0.8, 761);
    weatherOut.SetMeasurements(16, 0.60, 751);

    weatherIn.RemoveObserver(statsDisplay);
    weatherOut.RemoveObserver(statsDisplay);

    weatherIn.SetMeasurements(10, 0.8, 761);
    weatherOut.SetMeasurements(-10, 0.9, 740);

    return 0;
}