#ifndef CWINDCOUNTER_H
#define CWINDCOUNTER_H
#include <limits>
#include <string>
#include <iostream>
#include <cmath>

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

        AddAngle(data.direction);
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

    [[nodiscard]] double GetWindDirectionAverage() const
    {
        if (m_degreesCount == 0)
        {
            return 0;
        }

        const double avgRadians = atan2(m_sumSin, m_sumCos);
        double avgDegrees = avgRadians * 180.0 / M_PI;

        if (avgDegrees < 0)
        {
            avgDegrees += 360;
        }
        return avgDegrees;
    }

    void DisplayData(const std::string& name) const
    {
        std::cout << "Max " << name << " " << GetMaxWindSpeed() << std::endl;
        std::cout << "Min " << name << " " << GetMinWindSpeed() << std::endl;
        std::cout << "Average " << name << " " << GetWindSpeedAverage() << std::endl;
        std::cout << "Average " << name << " direction " << GetWindDirectionAverage() << " degrees" << std::endl;
    }

private:
    double m_windSpeedTotal = 0;
    double m_minWindSpeed = std::numeric_limits<double>::infinity();
    double m_maxWindSpeed = -std::numeric_limits<double>::infinity();
    int m_windCountData = 0;
    double m_sumSin = 0;
    double m_sumCos = 0;
    int m_degreesCount = 0;

    void AddAngle(const double angle)
    {
        const double radians = angle * M_PI / 180.0;
        m_sumSin += sin(radians);
        m_sumCos += cos(radians);
        m_degreesCount++;
    }
};

#endif //CWINDCOUNTER_H
