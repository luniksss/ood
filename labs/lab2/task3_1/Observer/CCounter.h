#ifndef CCOUNTER_H
#define CCOUNTER_H
#include <limits>
#include <string>
#include <iostream>

class CCounter
{
public:
    void ProcessData(double data)
    {
        if (m_minData > data)
        {
            m_minData = data;
        }
        if (m_maxData < data)
        {
            m_maxData = data;
        }
        m_sumData += data;
        ++m_countData;
    }

    double GetMinData() const
    {
        return m_minData;
    }

    double GetMaxData() const
    {
        return m_maxData;
    }

    double GetAverage() const
    {
        return m_sumData / m_countData;
    }

    void DisplayData(const std::string& name)
    {
        std::cout << "Max " << name << " " << GetMaxData() << std::endl;
        std::cout << "Min " << name << " " << GetMinData() << std::endl;
        std::cout << "Average " << name << " " << GetAverage() << std::endl;
    }

private:
    double m_minData = std::numeric_limits<double>::infinity();
    double m_maxData = -std::numeric_limits<double>::infinity();
    double m_sumData = 0;
    int m_countData = 0;
};

#endif //CCOUNTER_H
