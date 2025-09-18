#include "./Observable/CWeatherData.h"
#include "./Observer/CDisplay.h"
#include "./Observer/CStatsDisplay.h"

int main()
{
    CWeatherData wd;

    CDisplay display;
    wd.RegisterObserver(display, 1);

    CStatsDisplay statsDisplay;
    wd.RegisterObserver(statsDisplay, 2);

    SWindInfo wind = {3, 10};
    wd.SetMeasurements(3, 0.7, 760, wind);
    wd.SetMeasurements(4, 0.8, 761, wind);

    wd.RemoveObserver(statsDisplay);

    wind.direction = 100;
    wind.speed = 10;
    wd.SetMeasurements(10, 0.8, 761, wind);
    wd.SetMeasurements(-10, 0.8, 761, wind);
    return 0;
}