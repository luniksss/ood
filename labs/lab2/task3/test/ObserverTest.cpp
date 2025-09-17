#include <gtest/gtest.h>
#include "../SWeatherInfo.h"
#include "../Observer/IObserver.h"
#include "../Observable/CWeatherData.h"
#include <vector>

class TestObserver : public IObserver<SWeatherInfo>
{
public:
    std::vector<int> updates;
    int id;

    TestObserver(int id_) : id(id_) {}

    void Update(SWeatherInfo const& data) override
    {
        updates.push_back(id);
    }
};

TEST(CObservablePriorityTest, NotificationOrderAndNoDuplicates)
{
    CWeatherData weatherData;

    TestObserver obs1(1);
    TestObserver obs2(2);
    TestObserver obs3(3);

    weatherData.RegisterObserver(obs2, 5);
    weatherData.RegisterObserver(obs1, 10);
    weatherData.RegisterObserver(obs3, 1);

    weatherData.RegisterObserver(obs1, 10);
    weatherData.SetMeasurements(25.0, 50.0, 760.0);

    ASSERT_EQ(obs1.updates.size(), 1);
    ASSERT_EQ(obs2.updates.size(), 1);
    ASSERT_EQ(obs3.updates.size(), 1);

    std::vector<int> expectedOrder = {1, 2, 3};

    weatherData.RemoveObserver(obs2);
    obs1.updates.clear();
    obs2.updates.clear();
    obs3.updates.clear();

    weatherData.SetMeasurements(26.0, 55.0, 758.0);

    ASSERT_EQ(obs1.updates.size(), 1);
    ASSERT_EQ(obs2.updates.size(), 0);
    ASSERT_EQ(obs3.updates.size(), 1);
}
