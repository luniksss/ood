#ifndef CWINDCOUNTER_H
#define CWINDCOUNTER_H
#include <limits>
#include <string>
#include <iostream>

#include "../SWeatherInfo.h"

class CWindCounter
{
public:
    void ProcessData(const SWindInfo& data)
    {
        if (m_minWindSpeed > data.speed)
        {
            m_minWindSpeed = data.speed;
        }
        if (m_maxWindSpeed < data.speed)
        {
            m_maxWindSpeed = data.speed;
        }
        m_windSpeedTotal += data.speed;
        ++m_windCountData;

        //  TODO придумать отображение градусов
    }

    [[nodiscard]] double GetMinWindSpeed() const
    {
        return m_minWindSpeed;
    }

    [[nodiscard]] double GetMaxWindSpeed() const
    {
        return m_maxWindSpeed;
    }

    [[nodiscard]] double GetWindSpeedAverage() const
    {
        if (m_windCountData == 0)
        {
            return 0;
        }
        return m_windSpeedTotal / m_windCountData;
    }

    void DisplayData(const std::string& name) const
    {
        std::cout << "Max " << name << " " << GetMaxWindSpeed() << std::endl;
        std::cout << "Min " << name << " " << GetMinWindSpeed() << std::endl;
        std::cout << "Average " << name << " " << GetWindSpeedAverage() << std::endl;
    }

private:
    double m_windSpeedTotal = 0;
    double m_minWindSpeed = std::numeric_limits<double>::infinity();
    double m_maxWindSpeed = -std::numeric_limits<double>::infinity();
    int m_windCountData = 0;
};

#endif //CWINDCOUNTER_H
