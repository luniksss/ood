#ifndef CCOUNTER_H
#define CCOUNTER_H
#include <limits>
#include <string>
#include <iostream>

class CCounter
{
public:
    void ProcessData(const double data)
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

    [[nodiscard]] double GetMinData() const
    {
        return m_minData;
    }

    [[nodiscard]] double GetMaxData() const
    {
        return m_maxData;
    }

    [[nodiscard]] double GetAverage() const
    {
        return m_sumData / m_countData;
    }

private:
    double m_minData = std::numeric_limits<double>::infinity();
    double m_maxData = -std::numeric_limits<double>::infinity();
    double m_sumData = 0;
    int m_countData = 0;
};

#endif //CCOUNTER_H
