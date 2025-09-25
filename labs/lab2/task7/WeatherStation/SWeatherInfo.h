#ifndef SWEATHERINFO_H
#define SWEATHERINFO_H

struct SWindInfo
{
    double speed = 0;
    double direction = 0;
};

struct SWeatherInfo
{
    double temperature = 0;
    double humidity = 0;
    double pressure = 0;
    SWindInfo wind{};
};

#endif //SWEATHERINFO_H
