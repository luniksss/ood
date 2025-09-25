#ifndef CWEATHERDATA_H
#define CWEATHERDATA_H
#include "../SWeatherInfo.h"
#include "./CObservable.h"

class CWeatherData : public CObservable<SWeatherInfo, EWeatherEventType>
{
public:
    // Температура в градусах Цельсия
    [[nodiscard]] double GetTemperature() const
    {
        return m_temperature;
    }

    // Относительная влажность (0...100)
    [[nodiscard]] double GetHumidity() const
    {
        return m_humidity;
    }

    // Атмосферное давление (в мм.рт.ст)
    [[nodiscard]] double GetPressure() const
    {
        return m_pressure;
    }

    // Скорость и направление ветра
    [[nodiscard]] SWindInfo GetWindInfo() const
    {
        return m_wind;
    }

    void MeasurementsChanged(const EWeatherEventType eventType)
    {
        NotifyObservers(eventType);
    }

    void SetTemp(const double temp)
    {
        m_temperature = temp;
        MeasurementsChanged(EWeatherEventType::Temperature);
    }

    void SetHumidity(const double humidity)
    {
        m_humidity = humidity;
        MeasurementsChanged(EWeatherEventType::Humidity);
    }

    void SetPressure(const double pressure)
    {
        m_pressure = pressure;
        MeasurementsChanged(EWeatherEventType::Pressure);
    }

    void SetWind(const SWindInfo wind)
    {
        m_wind = wind;
        MeasurementsChanged(EWeatherEventType::Wind);
    }

    void SetMeasurements(const double temp, const double humidity, const double pressure, const SWindInfo wind)
    {
        m_humidity = humidity;
        m_temperature = temp;
        m_pressure = pressure;
        m_wind = wind;

        MeasurementsChanged(EWeatherEventType::Temperature);
        MeasurementsChanged(EWeatherEventType::Humidity);
        MeasurementsChanged(EWeatherEventType::Pressure);
        MeasurementsChanged(EWeatherEventType::Wind);
    }
protected:
    [[nodiscard]] SWeatherInfo GetChangedData() const override
    {
        SWeatherInfo info;
        info.temperature = GetTemperature();
        info.humidity = GetHumidity();
        info.pressure = GetPressure();
        info.wind = GetWindInfo();
        return info;
    }
private:
    double m_temperature = 0.0;
    double m_humidity = 0.0;
    double m_pressure = 760.0;
    SWindInfo m_wind{};
};

#endif //CWEATHERDATA_H
